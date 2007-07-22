#
#    $Id$
#

all:
	mkdir -p Apps/Development
	cp -r ../!Perl Apps/Development
	cp ../ReadMe Apps/Development/!Perl
	perl CreatePackage
	zip -r -9 -I ^.perl-pkg/zip Apps RiscPkg Sprites SysVars -x *svn* *Documents*
	perl CreatePackage doc
	zip -r -9 -I ^.perl-doc-pkg/zip Apps.Development.!Perl.Documents RiscPkg -x *svn*



.PHONY: clean all


clean:
	-rm -rf ../perl-pkg.zip ../perl-doc-pkg.zip Apps RiscPkg/Control
