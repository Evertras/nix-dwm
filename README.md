# Nix DWM

Fork of [dwm](https://dwm.suckless.org/).

Changes applied with nix templating in mind with [nix-system](https://github.com/Evertras/nix-systems).

Base dwm is saved in `base` branch with this README and a Makefile tweak to allow for easier patching.

To create a diff patch:

```bash
make
mv patch.diff ../somewhere
```
