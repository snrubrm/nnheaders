/**
 * @file account.h
 * @brief Account service implementation.
 */

#pragma once

#include <nn/os.h>
#include <nn/types.h>

namespace nn {
namespace account {
struct Nickname {
    char m_Buffer[0x21];
};
struct NetworkServiceAccountId {
    u64 m_Id;
};

class AsyncContext;

class Uid {
public:
    bool IsValid() const { return m_Storage[0] != 0 || m_Storage[1] != 0; }

    u64 m_Storage[2];
};

class UserHandle {
public:
    Uid m_Uid;
    void* m_Handle;
};

void Initialize();
Result ListAllUsers(s32*, Uid*, s32 numUsers);
Result OpenUser(UserHandle*, Uid const&);
Result IsNetworkServiceAccountAvailable(bool* out, UserHandle const&);
void CloseUser(UserHandle const&);

Result EnsureNetworkServiceAccountAvailable(UserHandle const& userHandle);
Result EnsureNetworkServiceAccountIdTokenCacheAsync(AsyncContext*, UserHandle const&);
Result LoadNetworkServiceAccountIdTokenCache(u64*, char*, u64, UserHandle const&);

Result GetLastOpenedUser(Uid*);
Result GetNickname(Nickname* nickname, Uid const& userID);
Result GetNetworkServiceAccountId(NetworkServiceAccountId*, const UserHandle&);

Result GetUserId(Uid* uid, const UserHandle& handle);
Result OpenPreselectedUser(UserHandle* handle);

class AsyncContext {
public:
    AsyncContext();
    ~AsyncContext();

    Result HasDone(bool*);
    Result GetResult();
    Result Cancel();
    Result GetSystemEvent(nn::os::SystemEvent*);

private:
    // 0x20 bytes: nn::nex::JobAcquireNsaIdToken constructs one at +0xa8 (0x7100ce5954) with
    // its next member at +0xc8; PosTrackerUploader 0x7100a8d2fc keeps one at sp+0x30.
    std::aligned_storage_t<0x20, 8> m_Storage;
};
}  // namespace account
}  // namespace nn
