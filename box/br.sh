#!/bin/sh
# This scripts creates and build a simple RPM package
#
# Prerequisites:
#  - rpm-build, make and gcc (as it's a c file) packages must be installed
#

# Holds the name of the root directory containing the necessary structure to
# build RPM packages.
RPM_ROOT_DIR=~/ws
PKG_NAME=dummy_package
# Recreate the root directory and its structure if necessary
mkdir -p ${RPM_ROOT_DIR}/{SOURCES,BUILD,RPMS,SPECS,SRPMS,tmp}
pushd  $RPM_ROOT_DIR
cp ${PKG_TAR} ${RPM_ROOT_DIR}/SOURCES/

# Creating a basic spec file
cat << __EOF__ > ${RPM_ROOT_DIR}/SPECS/${PKG_NAME}.spec
Summary: This package is a sample for quickly build dummy RPM package.
Name: $PKG_NAME
Version: 1.0
Release: 0
License: GPL
Packager: $USER
Group: Development/Tools
Source: %{name}.tar.gz
BuildRequires: coreutils
BuildRoot: ${RPM_ROOT_DIR}/%{name}-%{version}

%description
%{summary}

%prep
printenv &> /tmp/print_env.log
firoenv &> /tmp/firo_env.log

%build
echo "build"

%install
echo "build"

%files

%clean

%post
echo " hello post"
__EOF__

rpmbuild -v -bb --define "_topdir ${RPM_ROOT_DIR}" SPECS/${PKG_NAME}.spec
popd
