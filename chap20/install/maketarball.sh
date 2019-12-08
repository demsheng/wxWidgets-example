#!/bin/sh

# Make a distribution of AcmeApp for Linux and Mac
# (c) Julian Smart
# Usage: maketarball.sh [ options ]
# For example: maketarball.sh --build

PROGNAME=$0
CVSUPDATE=0
COPYTOFTP=0
# Clean objects before build
CLEANOBJECTS=1
DEBUGBUILD=0
RELEASEBUILD=0
NOTAR=0
CLEANWX=0

# Will be computed
VERSION=1.00

# This is i686 for Linux
PLATFORM=`arch`

# Read Linux or Mac variables
if [ "$PLATFORM" = "i686" ]; then
    . setup_unix.var
else
    . setup_mac.var
fi

# Find the version from symbols.h

find_version()
{
    echo "Finding version..."
    echo "#include <stdio.h>" > /tmp/appver.c
    echo "#include \"symbols.h\"" >> /tmp/appver.c
    echo "int main() { printf(\"%.2f\", VERSION_NUMBER); }" >> /tmp/appver.c
    gcc /tmp/appver.c -I$SRCDIR -o /tmp/appver
    VERSION=`/tmp/appver`

    echo "Version is "$VERSION
}

# Get the latest from the CVS server

update_from_cvs()
{
    # Update your project here
    cd $PROJECTDIR
    cvs update -d -P
}

# Copy to public ftp site

copy_to_ftp_site()
{
    if [ "$PLATFORM" = "i686" ]; then
        echo Copying tarball AcmeApp-$VERSION-i386.tar.gz to ftp site...

        curl ftp://www.acmecorp.com/public_html/AcmeApp-$VERSION-i386.tar.gz --user "username:password" --upload-file $DESTDIR/AcmeApp-$VERSION-i386.tar.gz
    else
        echo Copying tarball AcmeApp-$VERSION.dmg to ftp site...

        curl ftp://www.anthemion.co.uk/public_html/acmeapp/AcmeApp-$VERSION.dmg --user "username:password" --upload-file $DESTDIR/AcmeApp-$VERSION.dmg
    fi
}

# Test out the installation

install_app()
{
    echo Installing from $DESTDIR...
    rm -f -r $DESTDIR/tmp
    mkdir $DESTDIR/tmp
    cd $DESTDIR/tmp
    tar xvfz ../AcmeApp-$VERSION-i386.tar.gz
    ./installacme
}

# Make the tarball

make_linux_dist()
{

    rm -f -r $DESTDIR/*
    rm -f $DESTDIR/*

    mkdir -p $DESTDIR/AcmeApp-$VERSION
    cd $DESTDIR/AcmeApp-$VERSION

    cp $APPDIR/docs/readme-unix.txt readme.txt
    cp $APPDIR/docs/readme-unix.txt ../readme.txt
    cp $APPDIR/docs/license.txt .
    cp $APPDIR/src/bitmaps/acmeapp32x32.xpm .
    cp $APPBUILDDIR/acmeapp .

    echo Copying manuals etcetera...
    cp $APPDIR/manual/acmeapp.htb .

    mkdir -p Samples
    echo Copying sample files...
    cp -r $APPDIR/Samples .

    cd $DESTDIR/AcmeApp-$VERSION

    echo Zipping up resources...
    cp -r $APPDIR/resources resources

    cd $DESTDIR/AcmeApp-$VERSION/resources

    rm -f ../acmeapp.bin

    zip -q ../acmeapp.bin *

    cd ..

    rm -f -r resources

    mv acmeapp.bin $DESTDIR/AcmeApp-$VERSION

    cd $DESTDIR/AcmeApp-$VERSION

    strip acmeapp

    if [ "$UPX" != "0" ]; then
        upx acmeapp
    fi

    ###################### COPY SCRIPTS

    # Modify the installer script to know about the version

    sed "s/__ACMEVERSION__/$VERSION/g" < $APPDIR/scripts/installacme > $DESTDIR/installacme
    chmod a+x $DESTDIR/installacme

    ###################### TAR UP WRITER'S CAFE

    # We archive this data archive inside the outer archive
    # _without_ a versioned folder, so it can be put in the
    # destination folder of choice.

    cd $DESTDIR/AcmeApp-$VERSION

    tar zcvf $DESTDIR/AcmeAppData.tar.gz *

    cd $DESTDIR

    tar zcvf AcmeApp-$VERSION-i386.tar.gz AcmeAppData.tar.gz installacme

    rm -f AcmeAppData.tar.gz
}

make_mac_dist()
{
    mkdir -p $DESTDIR
    cd $DESTDIR

    echo Removing old files...
    rm -f -r $DESTDIR/AcmeApp*
    rm -f *.dmg

    echo Copying AcmeApp.app...
    mkdir -p $DESTDIR/AcmeApp-$VERSION
    ditto --rsrc $APPBUILDDIR/AcmeApp.app $DESTDIR/AcmeApp-$VERSION/AcmeApp.app
    strip $DESTDIR/AcmeApp-$VERSION/AcmeApp.app/Contents/MacOS/AcmeApp

    cd $DESTDIR/AcmeApp-$VERSION

    echo Copying readme files...
    cp $APPDIR/docs/readme-mac.txt readme.txt
    cp $APPDIR/docs/license.txt .

    echo Copying manuals etcetera...
    cp $APPDIR/manual/acmeapp.htb AcmeApp.app/Contents/MacOS

    echo Copying samples...
    cp -r $APPDIR/Samples .

    echo Zipping binary resource file...
    cd $DESTDIR/AcmeApp-$VERSION

    cp -r $APPDIR/resources resources

    cd $DESTDIR/AcmeApp-$VERSION/resources

    rm -f ../acmeapp.bin 

    zip -q ../acmeapp.bin *

    cd ..

    mv acmeapp.bin $DESTDIR/AcmeApp-$VERSION/AcmeApp.app/Contents/MacOS

    rm -f -r resources

    cd $DESTDIR

    ############################### FINISH OFF THE DISTRIBUTION

    echo Making a disk image...
    hdiutil create AcmeApp-$VERSION.dmg -volname AcmeApp-$VERSION -type UDIF -megabytes 50 -fs HFS+

    echo Mounting the disk image...
    MYDEV=`hdiutil attach AcmeApp-$VERSION.dmg | tail -n 1 | awk '{print $1'}`
    echo Device is $MYDEV

    echo Copying AcmeApp to the disk image...
    ditto --rsrc AcmeApp-$VERSION /Volumes/AcmeApp-$VERSION/AcmeApp-$VERSION

    echo Unmounting the disk image...
    hdiutil detach $MYDEV

    echo Compressing the disk image...
    hdiutil convert AcmeApp-$VERSION.dmg -format UDZO -o AcmeApp-$VERSION-compressed.dmg

    echo Internet enabling the disk image...
    hdiutil internet-enable AcmeApp-$VERSION-compressed.dmg

    echo Renaming compressed image...
    rm -f AcmeApp-$VERSION.dmg
    mv AcmeApp-$VERSION-compressed.dmg AcmeApp-$VERSION.dmg
}

# Build all Acme App applications

do_build()
{
    cd $BUILDDIR
    
    # Insert build code here
}

do_build_debug()
{
    cd $BUILDDIR
    
    # Insert build code here
}

usage()
{
    echo "Makes tarballs and optionally rebuilds Acme App applications."
    echo "Usage: $PROGNAME [ options ]"
    echo Options:
    echo "    --help               Display this help message"
    echo "    --build              Cleans and builds Acme App first"
    echo "    --debug-build        Cleans and debug-builds Acme App first"
    echo "    --cvs-update         Update from CVS first"
    echo "    --ftp                Copy to ftp site"
    echo "    --noclean            Resume build without cleaning objects"
    echo "    --cleanwx            Clean wxWidgets build first"
    echo "    --install            Install after building the distribution"
    echo "    --notar              Omit the distribution creation"
    exit 1
}

# Process command line options.
for i in "$@"; do
    case "$i" in
	--build) RELEASEBUILD=1 ;;
	--debug-build) DEBUGBUILD=1 ;;
	--cvs-update) CVSUPDATE=1 ;;
	--ftp) COPYTOFTP=1 ;;
	--noclean) CLEANOBJECTS=0 ;;
	--cleanwx) CLEANWX=1 ;;
	--notar) NOTAR=1 ;;
	--install) INSTALLWC=1 ;;
	*)
	    usage
	    exit
	    ;;
    esac
done

if [ "$CVSUPDATE" = "1" ]; then
    update_from_cvs
fi

find_version

if [ ! -d "$DESTDIR" ]; then
    mkdir -p $DESTDIR
fi

if [ ! -d "$SRCDIR" ]; then
    echo Source directory $SRCDIR not found.
    usage
    exit 1
fi

find_version

if [ "$NOTAR" = "0" ]; then
  echo Creating Version $VERSION distribution in $DESTDIR, using source directory $SRCDIR and acmeapp binary in $APPBUILDDIR.

  # Remove all existing files
  if [ ! -d "$DESTDIR/acmeapp" ]; then
      rm -f -r $DESTDIR/acmeapp
  fi
fi

if [ "$CLEANWX" = "1" ]; then
    clean_wx
fi

if [ "$RELEASEBUILD" = "1" ] || [ "$BUILDDESKONLY" = "1" ]; then
    do_build
fi

if [ "$DEBUGBUILD" = "1" ]; then
    do_build_debug
fi

if [ ! -d "$APPBUILDDIR" ]; then
    echo Location of acmeapp binary $APPBUILDDIR not found.
    usage
    exit 1
fi

if [ ! -f $APPBINARY ]; then
  echo "*** Sorry, Acme was not built correctly."
  exit
fi

if [ "$NOTAR" = "0" ]; then
    if [ "$PLATFORM" = "i686" ]; then
        make_linux_dist
    else
        make_mac_dist
    fi
fi

if [ "$COPYTOFTP" = "1" ]; then
    copy_to_ftp_site
fi

if [ "$INSTALLWC" = "1" ]; then
    install_app
fi

echo AcmeApp archived.
