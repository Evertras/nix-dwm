# dwm - dynamic window manager
# See LICENSE file for copyright and license details.

# Completely modified to just focus on patching
.PHONY: patch
patch:
	git reset --soft base
	git checkout -b tmp-patch
	git add -A
	git commit -m "Changes"
	git format-patch base --stdout > patch.diff
	git checkout main
	git branch -D tmp-patch
