#ToggleBetweenDesktops#
---

Supplies a new shortcut which enables you to switch between
the current desktop and the previous (hence the "toggle") desktop.

This is not activity-aware. Meaning, that it does not store the
desktops (curr + prev) when changing activities.

Apply patches to: **kde-workspace/kwin/**

Restart KWin with: kwin --replace