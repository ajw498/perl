#
#    $Id$
#

release: clean
	zip -r -9 -I perl/zip !Perl License ReadMe Source -x *svn*
	make -C pkg

.PHONY: clean release


clean:
	-rm -rf perl.zip
	make -C pkg clean
