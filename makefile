CPPFLAGS=`pkg-config --libs libcurl` 
LIBS="-lpthread"
MYSQLLIBS=`mysql_config --cflags --libs`
MYSQLINC=`mysql_config --cflags`
TARNAME=wtfwms51-`date +"%y.%m.%d"`
DEB=`pwd`/debBuild
ARCH=`uname -i`
DODEBUG=#-g
DOPROFILE=#-pg
all: wtfwms wtfworker

wtfwms: src/wtfwmsd.cpp src/wtfwmsd.h
	g++ $(DODEBUG) $(DOPROFILE) -o usr/bin/wtfwmsd src/wtfwmsd.cpp

bin/cpuload.o: src/cpuload.cpp src/cpuload.h
	g++ -c $(DODEBUG) $(DOPROFILE) src/cpuload.cpp -o bin/cpuload.o

bin/config.o: src/config.cpp src/config.h
	g++ -c $(DODEBUG) $(DOPROFILE) src/config.cpp -o bin/config.o

bin/netlink.o: src/netlink.cpp src/netlink.h
	g++ -c $(DODEBUG) $(DOPROFILE) src/netlink.cpp -o bin/netlink.o

bin/trace.o: src/trace.cpp src/trace.h bin/netlink.o
	g++ -c $(DODEBUG) $(DOPROFILE) src/trace.cpp $(MYSQLINC) -o bin/trace.o

bin/threads.o: src/threads.cpp src/threads.h
	g++ -c $(DODEBUG) $(DOPROFILE) src/threads.cpp $(MYSQLINC) -o bin/threads.o

bin/apache.o: src/apache.cpp src/apache.h bin/config.o bin/trace.o
	g++ -c $(DODEBUG) $(DOPROFILE) src/apache.cpp $(MYSQLINC) -o bin/apache.o

bin/postdata.o: src/postdata.cpp src/postdata.h bin/config.o
	g++ -c $(DODEBUG) $(DOPROFILE) src/postdata.cpp $(MYSQLINC) -o bin/postdata.o

bin/syscalls.o: src/syscalls.cpp src/syscalls.h
	g++ -c $(DODEBUG) $(DOPROFILE) src/syscalls.cpp $(MYSQLINC) -o bin/syscalls.o

bin/my_mysql.o: src/my_mysql.cpp src/my_mysql.h
	g++ -c $(DODEBUG) $(DOPROFILE) src/my_mysql.cpp $(MYSQLINC) -o bin/my_mysql.o


wtfworker: bin/cpuload.o src/main.cpp src/main.h bin/apache.o bin/config.o bin/syscalls.o bin/threads.o bin/postdata.o bin/my_mysql.o bin/netlink.o
	g++ $(DODEBUG) $(DOPROFILE) bin/config.o bin/apache.o bin/cpuload.o bin/netlink.o bin/trace.o bin/threads.o bin/postdata.o bin/syscalls.o bin/my_mysql.o -o usr/bin/wtfworker src/main.cpp  `pkg-config --libs libcurl` -lpthread $(MYSQLLIBS)

clean:
	rm bin/* -rf
	rm usr/bin/* -rf
	rm wtfwms-*.tar.gz -rf
	rm wtfwms-*.tar.gz -rf
	rm /home/makerpm/rpmbuild/RPMS/$(ARCH)/* -rf
tar: clean
	mkdir $(TARNAME)
	cp src $(TARNAME) -r
	rm $(TARNAME)/src/.svn -rf
	cp bin $(TARNAME) -r
	rm $(TARNAME)/bin/.svn -rf
	cp usr $(TARNAME) -r
	rm $(TARNAME)/usr/.svn -rf
	rm $(TARNAME)/usr/bin/.svn -rf
	cp etc $(TARNAME) -r
	rm $(TARNAME)/etc/.svn -rf
	rm $(TARNAME)/etc/init.d/.svn -rf
	cp makefile $(TARNAME) -r
	tar -cf $(TARNAME).tar $(TARNAME)
	gzip $(TARNAME).tar
	rm $(TARNAME) -rf

rpm: clean tar
	rm /home/makerpm/rpmbuild/SOURCES/* -rf
	cp /home/makerpm/wtfwms/*.tar.gz /home/makerpm/rpmbuild/SOURCES/
	cd /home/makerpm/rpmbuild/SPECS
	rm /home/makerpm/rpmbuild/RPMS/$(ARCH)/wtfwms* -rf
	rpmbuild -ba wtfwms-0.0.1.spec
	rm /home/makerpm/rpmbuild/RPMS/$(ARCH)/*debuginfo* -rf
	rpm --addsign /home/makerpm/rpmbuild/RPMS/$(ARCH)/wtfwms*
	cp /home/makerpm/rpmbuild/RPMS/$(ARCH)/wtfwms* /home/makerpm/wtfwms/RPMs/$(ARCH)/

install:
	install -s -m 755 usr/bin/wtfwmsd /usr/bin/wtfwmsd
	install -s -m 755 usr/bin/wtfworker /usr/bin/wtfworker
	install -m 755 etc/wtfwms.ini.sample /etc/wtfwms.ini.sample
	install -m 755 etc/init.d/wtfwms /etc/init.d/wtfwms

deb51: clean all
	rm -rf $(DEB)
	mkdir $(DEB) 
	mkdir $(DEB)/DEBIAN 
	cp usr $(DEB) -r
	rm $(DEB)/usr/.svn -rf
	rm $(DEB)/usr/bin/.svn -rf
	cp etc $(DEB) -r
	rm $(DEB)/etc/.svn -rf
	rm $(DEB)/etc/init.d/.svn -rf
	cp $(DEB)/../debian/control51 $(DEB)/DEBIAN/control
	cp $(DEB)/../debian/conffiles $(DEB)/DEBIAN
	cp $(DEB)/../debian/wtfwms.ini.sample $(DEB)/etc -f
	chmod ugo+x $(DEB)/etc/init.d/wtfwms
	dpkg-deb --build $(DEB)
	mv debBuild.deb DEBs/wtfwms51-`date +"%y.%m.%d"`.`uname -n`.`arch`.deb
	rm -rf $(DEB)

deb55: clean all
	rm -rf $(DEB)
	mkdir $(DEB) 
	mkdir $(DEB)/DEBIAN 
	cp usr $(DEB) -r
	rm $(DEB)/usr/.svn -rf
	rm $(DEB)/usr/bin/.svn -rf
	cp etc $(DEB) -r
	rm $(DEB)/etc/.svn -rf
	rm $(DEB)/etc/init.d/.svn -rf
	cp $(DEB)/../debian/control55 $(DEB)/DEBIAN/control
	cp $(DEB)/../debian/conffiles $(DEB)/DEBIAN
	cp $(DEB)/../debian/wtfwms.ini.sample $(DEB)/etc -f
	chmod ugo+x $(DEB)/etc/init.d/wtfwms
	dpkg-deb --build $(DEB)
	mv debBuild.deb DEBs/wtfwms55-`date +"%y.%m.%d"`.`uname -n`.`arch`.deb
	rm -rf $(DEB)

