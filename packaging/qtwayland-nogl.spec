# Version from git
%define _qtmodule_snapshot_version 0.0.alpha1.16.gb62ae0f

# Package prefix
%define pkgname qt5-qtwayland-nogl

Name:           qtwayland-nogl
Summary:        Qt Wayland, nogl variant
#Version:        %{_qtmodule_snapshot_version}
Version:        5.3.90
Release:        1
Group:          Qt/Qt
License:        LGPLv2.1 with exception or GPLv3
URL:            http://qt.io
Source0:        %{name}-%{version}.tar.xz
Source100:      precheckin.sh
Patch0:         qtwayland-brcm_egl-force-vc-include-bcm-libs.patch
Patch1:         qtwayland-brcm_egl-fix-build.patch
Patch2:         qtwayland-nogl-fixbuild.patch
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Widgets)
BuildRequires:  pkgconfig(Qt5OpenGL)
BuildRequires:  pkgconfig(Qt5PlatformSupport)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5DBus)

BuildRequires:  pkgconfig(wayland-client)
BuildRequires:  pkgconfig(wayland-server)
BuildRequires:  pkgconfig(wayland-cursor)
%if "%{name}" == "qt5-qtwayland-brcm_egl"
BuildRequires:  xorg-x11-proto-xproto
%endif
%if "%{name}" == "qt5-qtwayland-nogl"
BuildRequires:  pkgconfig(wayland-egl)
%endif
%if "%{name}" == "qt5-qtwayland-wayland_egl"
BuildRequires:  pkgconfig(wayland-egl)
%endif

BuildRequires:  qtchooser
BuildRequires:  libxkbcommon-devel
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  libffi-devel
BuildRequires:  fdupes

Requires:       xkeyboard-config

%description
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.

This package contains the Qt wayland compositor for nogl.


%package -n %{pkgname}
Summary:        Qt Wayland for applications and compositors
Group:          Qt/Qt

%description -n %{pkgname}
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.

This package contains the Qt wayland compositor for nogl.


%package -n %{pkgname}-devel
Summary:        Qt Wayland for applications and compositors - development files
Group:          Qt/Qt
Requires:       %{pkgname} = %{version}-%{release}

%description -n %{pkgname}-devel
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.

This package contains the Qt wayland compositor development
files for nogl.


%package -n %{pkgname}-examples
Summary:        Qt Wayland compositor - examples
Group:          Qt/Qt
Requires:       %{pkgname} = %{version}-%{release}

%description -n %{pkgname}-examples
Qt is a cross-platform application and UI framework. Using Qt, you can
write web-enabled applications once and deploy them across desktop,
mobile and embedded systems without rewriting the source code.

This package contains the Qt wayland compositor examples for nogl.


%prep
%setup -q -n %{name}-%{version}
%if "%{name}" == "qt5-qtwayland-brcm_egl"
%patch0 -p1
%patch1 -p1
%endif
%if "%{name}" == "qt5-qtwayland-nogl"
%patch2 -p1
%endif


%build
export QTDIR=/usr/share/qt5
export QT_WAYLAND_GL_CONFIG=nogl
touch .git
%qmake5 QT_BUILD_PARTS+=examples CONFIG+=wayland-compositor

make %{?_smp_mflags}


%install
rm -rf %{buildroot}
%qmake_install

# Workaround to install generated private headers
cp ./include/QtCompositor/5.5.0/QtCompositor/private/{qwayland-server-*,*protocol*}.h \
    %{buildroot}/%{_includedir}/qt5/QtCompositor/5.5.0/QtCompositor/private/

rm %{buildroot}%{_libdir}/cmake/Qt5Gui/Qt5Gui_.cmake

# Fix wrong path in pkgconfig files
find %{buildroot}%{_libdir}/pkgconfig -type f -name '*.pc' \
    -exec perl -pi -e "s, -L%{_builddir}/?\S+,,g" {} \;
# Fix wrong path in prl files
find %{buildroot}%{_libdir} -type f -name '*.prl' \
    -exec sed -i -e "/^QMAKE_PRL_BUILD_DIR/d;s/\(QMAKE_PRL_LIBS =\).*/\1/" {} \;

# We don't need qt5/Qt/
rm -rf %{buildroot}/%{_includedir}/qt5/Qt

%fdupes %{buildroot}/%{_includedir}


%post -n %{pkgname} -p /sbin/ldconfig
%postun -n %{pkgname} -p /sbin/ldconfig


%files -n %{pkgname}
%defattr(-,root,root,-)
%{_libdir}/libQt5Compositor.so.5
%{_libdir}/libQt5Compositor.so.5.*
%{_libdir}/libQt5WaylandClient.so.5
%{_libdir}/libQt5WaylandClient.so.5.*
%{_libdir}/qt5/plugins/platforms/libqwayland-generic.so

%if "%{name}" == "qt5-qtwayland-brcm_egl"
%{_libdir}/qt5/plugins/platforms/libqwayland-brcm-egl.so
%{_libdir}/qt5/plugins/wayland-graphics-integration-server/libbrcm-egl.so
%{_libdir}/qt5/plugins/wayland-graphics-integration-client/libbrcm-egl.so
%endif

%if "%{name}" == "qt5-qtwayland-wayland_egl"
%{_libdir}/qt5/plugins/platforms/libqwayland-egl.so
%{_libdir}/qt5/plugins/wayland-graphics-integration-client/libdrm-egl-server.so
%{_libdir}/qt5/plugins/wayland-graphics-integration-client/libwayland-egl.so
%{_libdir}/qt5/plugins/wayland-graphics-integration-server/libdrm-egl-server.so
%{_libdir}/qt5/plugins/wayland-graphics-integration-server/libwayland-egl.so
%endif

%if "%{name}" == "qt5-qtwayland-xcomposite_egl"
%{_libdir}/qt5/plugins/platforms/libqwayland-xcomposite-egl.so
%{_libdir}/qt5/plugins/wayland-graphics-integration/libxcomposite-egl.so
%endif

%if "%{name}" == "qt5-qtwayland-nogl"
%{_libdir}/qt5/plugins/platforms/libqwayland-nogl.so
%endif

%files -n %{pkgname}-devel
%defattr(-,root,root,-)
%{_libdir}/libQt5Compositor.so
%{_includedir}/qt5/*
%{_libdir}/libQt5Compositor.la
%{_libdir}/libQt5Compositor.prl
%{_libdir}/pkgconfig/Qt5Compositor.pc
%{_libdir}/cmake/Qt5Compositor/*
%{_datadir}/qt5/mkspecs/modules/qt_lib_waylandclient.pri
%{_datadir}/qt5/mkspecs/modules/qt_lib_waylandclient_private.pri
%{_datadir}/qt5/mkspecs/modules/qt_lib_compositor.pri
%{_datadir}/qt5/mkspecs/modules/qt_lib_compositor_private.pri
%{_libdir}/libQt5WaylandClient.so
%{_libdir}/libQt5WaylandClient.la
%{_libdir}/libQt5WaylandClient.prl
%{_libdir}/pkgconfig/Qt5WaylandClient.pc
%{_libdir}/cmake/Qt5WaylandClient/*
%{_libdir}/qt5/bin/qtwaylandscanner

%files -n %{pkgname}-examples
%defattr(-,root,root,-)
%{_libdir}/qt5/examples/qtwayland/
