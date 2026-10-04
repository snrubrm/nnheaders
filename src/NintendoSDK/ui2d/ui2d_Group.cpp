#include <nn/ui2d/Group.h>

#include <new>
#include <nn/ui2d/Layout.h>

namespace nn::ui2d {

// inline-only in the original; name is a guess (a bounded name comparison: names are at most 0x20 bytes)
static bool IsNameEqual(const char* a, const char* b) {
    for (s32 i = 0; i < 0x20; i++) {
        if (a[i] != b[i])
            return false;
        if (a[i] == 0)
            return true;
    }
    return true;
}

// 0x7100ab629c
Group::Group(const char* name) : mName(name), mIsUserAllocated(false) {}

// 0x7100ab63d8
Group::~Group() {
    PaneLinkList::iterator it = mPaneLinkList.begin();
    while (it != mPaneLinkList.end()) {
        PaneLink& link = *it;
        mPaneLinkList.erase(it++);
        Layout::FreeMemory(&link);
    }
}

// 0x7100ab638c
void Group::AppendPane(Pane* pane) {
    void* memory = Layout::AllocateMemory(sizeof(PaneLink), 4);
    if (memory) {
        PaneLink* link = new (memory) PaneLink();
        link->pane = pane;
        mPaneLinkList.push_back(*link);
    }
}

// 0x7100ab64c8
GroupContainer::~GroupContainer() {
    GroupList::iterator it = mGroupList.begin();
    while (it != mGroupList.end()) {
        GroupList::iterator current = it++;
        mGroupList.erase(current);
        if (!current->IsUserAllocated()) {
            current->~Group();
            Layout::FreeMemory(&*current);
        }
    }
}

// 0x7100ab6550
void GroupContainer::AppendGroup(Group* group) {
    mGroupList.push_back(*group);
}

// 0x7100ab6570
Group* GroupContainer::FindGroupByName(const char* name) {
    for (Group& group : mGroupList) {
        if (IsNameEqual(group.GetName(), name))
            return &group;
    }
    return nullptr;
}

}  // namespace nn::ui2d
