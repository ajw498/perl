#
#    $Id$
#

all:
	mkdir -p Apps/Programming
	cp -r ../!Perl Apps/Programming
	cp ../ReadMe Apps/Programming/!Perl
	perl CreatePackage
	zip -r -9 -I ^.perl-pkg/zip Apps RiscPkg Sprites SysVars -x *svn* *Documents*
	perl CreatePackage doc
	zip -r -9 -I ^.perl-doc-pkg/zip Apps.Programming.!Perl.Documents RiscPkg -x *svn*



.PHONY: clean all


clean:
	-rm -rf ../perl-pkg.zip ../perl-doc-pkg.zip Apps RiscPkg/Control
