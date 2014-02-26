Summary: WTFWtihMyServer.com daemon. Monitors web-apps health and finds bottlenecks.
Name: wtfwms51
Version: 13.04.13
Release: 1%{?dist}

URL: http://wtfwithmyserver.com
Group: System Environment/Daemons
Source0: %{name}-%{version}.tar.gz
BuildRoot: %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
License: commercial

%description
WTFWtihMyServer.com daemon description goes here.

%prep
%setup -q

%build
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
mkdir -p $RPM_BUILD_ROOT/etc
mkdir -p $RPM_BUILD_ROOT/etc/init.d

install -s -m 755 usr/bin/wtfwmsd $RPM_BUILD_ROOT/usr/bin/wtfwmsd
install -s -m 755 usr/bin/wtfworker $RPM_BUILD_ROOT/usr/bin/wtfworker
install -m 755 etc/wtfwms.ini.sample $RPM_BUILD_ROOT/etc/wtfwms.ini.sample
install -m 755 etc/init.d/wtfwms $RPM_BUILD_ROOT/etc/init.d/wtfwms

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc src/README src/ChangeLog

/usr/bin/wtfwmsd
/usr/bin/wtfworker
/etc/wtfwms.ini.sample
/etc/init.d/wtfwms

%changelog
* Sat Mar 02 2013 Alex Kuzmuk <alex@kuzmuk.com>
- initial package (build 1)
