#!/bin/sh

for x in nogl wayland_egl xcomposite_egl brcm_egl; do
	cp qtwayland.spec qtwayland-$x.spec
	sed -i "s/custom_egl/$x/g" qtwayland-$x.spec
done
