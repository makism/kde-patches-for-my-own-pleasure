#RememberDesktopWhenSwitchingActivity#

patch submitted for review at: [https://git.reviewboard.kde.org/r/104261/](https://git.reviewboard.kde.org/r/104261/)

---

See kde bugs:

* [https://bugs.kde.org/show_bug.cgi?id=265015](https://bugs.kde.org/show_bug.cgi?id=265015)
* [https://bugs.kde.org/show_bug.cgi?id=241864](https://bugs.kde.org/show_bug.cgi?id=241864)
 and possibly more...

The problem is as follows:

1.  Say you have two (or more activities) A and B.
2.  You are working on activity A on Desktop 4.
3.  You switch to activity B (and by default to Desktop 4).
4.  Change to Desktop 1.
5.  Go back to activity A and (by default) to Desktop 1, while it should move you to Desktop 4 (this is where my patch kicks in).
  
Apply patches to: **kactivities/service/**

Kill and restart kactivitymanagerd
