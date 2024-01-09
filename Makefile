# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

# Completely modified to just focus on patching
.PHONY: patch
patch:
	git reset --soft base
	git add -A
	git commit -m "Changes"
	git format-patch base --stdout > patch.diff
