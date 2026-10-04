/**
 * @file Group.h
 * @brief Named groups of panes.
 */

#pragma once

#include <nn/types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::ui2d {

class Pane;

// An entry of a group's pane list (allocated with Layout::AllocateMemory, 0x18 bytes).
struct PaneLink {
    util::IntrusiveListNode link;
    Pane* pane;
};

// Layout evidence: Group::Group (0x7100ab629c), AppendPane (0x7100ab638c) and the destructor (0x7100ab63d8): the group
// is linked into its container's list at +8, owns a list of PaneLinks at +0x18, and has the (unowned) name at +0x28.
class Group {
public:
    typedef util::IntrusiveList<PaneLink, util::IntrusiveListMemberNodeTraits<PaneLink, &PaneLink::link>>
        PaneLinkList;

    explicit Group(const char* name);
    virtual ~Group();

    void AppendPane(Pane* pane);

    const char* GetName() const { return mName; }
    bool IsUserAllocated() const { return mIsUserAllocated; }

    util::IntrusiveListNode mLink;
    PaneLinkList mPaneLinkList;
    const char* mName;
    bool mIsUserAllocated;
};
static_assert(sizeof(Group) == 0x38);

// The groups of a layout (an intrusive list of Group, the first member of the owner; destroys the groups it owns).
class GroupContainer {
public:
    typedef util::IntrusiveList<Group, util::IntrusiveListMemberNodeTraits<Group, &Group::mLink>>
        GroupList;

    ~GroupContainer();

    void AppendGroup(Group* group);
    Group* FindGroupByName(const char* name);

private:
    GroupList mGroupList;
};

}  // namespace nn::ui2d
