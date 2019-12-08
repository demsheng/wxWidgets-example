#!/bin/sh

# Make a distribution of Riffle for Linux and Mac
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

# Expand lines in a zip file
expandlines()
{
    toexpand=$1
    outputfile=$2

    rm -f $outputfile
    touch $outputfile
    for line in `cat $toexpand` ; do
      if [ "$line" != "" ]; then
        ls $line >> $outputfile
      fi
    uniq < $outputfile > /tmp/uniqtemp.txt
    mv /tmp/uniqtemp.txt $outputfile
    done
}

# Find the version from symbols.h

find_version()
{
    echo "Finding version..."
    echo "#include <stdio.h>" > /tmp/appver.c
    echo "#include \"symbols.h\"" >> /tmp/appver.c
    echo "int main() { printf(\"%.2f\", rfVERSION_NUMBER); }" >> /tmp/appver.c
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
        echo Copying tarball Riffle-$VERSION-i386.tar.gz to ftp site...

        curl ftp://www.acmecorp.com/public_html/Riffle-$VERSION-i386.tar.gz --user "username:password" --upload-file $DESTDIR/Riffle-$VERSION-i386.tar.gz
    else
        echo Copying tarball Riffle-$VERSION.dmg to ftp site...

        curl ftp://www.anthemion.co.uk/public_html/riffle/Riffle-$VERSION.dmg --user "username:password" --upload-file $DESTDIR/Riffle-$VERSION.dmg
    fi
}

# Test out the installation

install_app()
{
    echo Installing from $DESTDIR...
    rm -f -r $DESTDIR/tmp
    mkdir $DESTDIR/tmp
    cd $DESTDIR/tmp
    tar xvfz ../Riffle-$VERSION-i386.tar.gz
    ./installriffle
}

# Make the tarball

make_linux_dist()
{

    rm -f -r $DESTDIR/*
    rm -f $DESTDIR/*

    mkdir -p $DESTDIR/Riffle-$VERSION
    cd $DESTDIR/Riffle-$VERSION

    cp $APPDIR/docs/readme.txt readme.txt
    cp $APPDIR/docs/readme.txt ../readme.txt
    cp $APPDIR/docs/license.txt .
    cp $APPDIR/src/bitmaps/riffle32x32.xpm .
    cp $APPBUILDDIR/riffle .

    echo Copying manuals etcetera...
    cp $APPDIR/manual/riffle.htb .

    mkdir -p Samples
    echo Copying sample files...
    cp $APPDIR/Samples/*.jpg Samples

    # Make source archive
    cd $APPDIR
    expandlines scripts/source.rsp /tmp/rifflesource.txt

    zip -@ -r $DESTDIR/Riffle-$VERSION/RiffleSource.zip < /tmp/rifflesource.txt

#    cd $DESTDIR/Riffle-$VERSION

#    echo Zipping up resources...
#    cp -r $APPDIR/resources resources

#    cd $DESTDIR/Riffle-$VERSION/resources

#    rm -f ../riffle.bin

#    zip -q ../riffle.bin *

#    cd ..

#    rm -f -r resources

#    mv riffle.bin $DESTDIR/Riffle-$VERSION

    cd $DESTDIR/Riffle-$VERSION

    strip riffle

    if [ "$UPX" != "0" ]; then
        upx riffle
    fi

    ###################### COPY SCRIPTS

    # Modify the installer script to know about the version

    sed "s/__RIFFLEVERSION__/$VERSION/g" < $APPDIR/scripts/installriffle > $DESTDIR/installriffle
    chmod a+x $DESTDIR/installriffle

    ###################### TAR UP WRITER'S CAFE

    # We archive this data archive inside the outer archive
    # _without_ a versioned folder, so it can be put in the
    # destination folder of choice.

    cd $DESTDIR/Riffle-$VERSION

    tar zcvf $DESTDIR/RiffleData.tar.gz *

    cd $DESTDIR

    tar zcvf Riffle-$VERSION-i386.tar.gz RiffleData.tar.gz installriffle

    rm -f RiffleData.tar.gz
}

make_mac_dist()
{
    mkdir -p $DESTDIR
    cd $DESTDIR

    echo Removing old files...
    rm -f -r $DESTDIR/Riffle*
    rm -f *.dmg

    echo Copying Riffle.app...
    mkdir -p $DESTDIR/Riffle-$VERSION
    ditto --rsrc $APPBUILDDIR/riffle.app $DESTDIR/Riffle-$VERSION/Riffle.app
    strip $DESTDIR/Riffle-$VERSION/Riffle.app/Contents/MacOS/riffle
    cp $APPDIR/scripts/Info.plist $DESTDIR/Riffle-$VERSION/Riffle.app/Contents

    cd $DESTDIR/Riffle-$VERSION

    echo Copying readme files...
    cp $APPDIR/docs/readme-mac.txt readme.txt
    cp $APPDIR/docs/license.txt .

    echo Copying manuals etcetera...
    cp $APPDIR/manual/riffle.htb Riffle.app/Contents/MacOS
    cp $APPDIR/src/bitmaps/riffle-app.icns Riffle.app/Contents/Resources

    echo Copying samples...
    cp -r $APPDIR/Samples .

    # Make source archive
    cd $APPDIR
    zip -@ -r $DESTDIR/Riffle-$VERSION/RiffleSource.zip < scripts/source.rsp

#    echo Zipping binary resource file...
#    cd $DESTDIR/Riffle-$VERSION

#    cp -r $APPDIR/resources resources

#    cd $DESTDIR/Riffle-$VERSION/resources

#    rm -f ../riffle.bin 

#    zip -q ../riffle.bin *

#    cd ..

#    mv riffle.bin $DESTDIR/Riffle-$VERSION/Riffle.app/Contents/MacOS

#    rm -f -r resources

    cd $DESTDIR

    ############################### FINISH OFF THE DISTRIBUTION

    echo Making a disk image...
    hdiutil create Riffle-$VERSION.dmg -volname Riffle-$VERSION -type UDIF -megabytes 50 -fs HFS+

    echo Mounting the disk image...
    MYDEV=`hdiutil attach Riffle-$VERSION.dmg | tail -n 1 | awk '{print $1'}`
    echo Device is $MYDEV

    echo Copying Riffle to the disk image...
    ditto --rsrc Riffle-$VERSION /Volumes/Riffle-$VERSION/Riffle-$VERSION

    echo Unmounting the disk image...
    hdiutil detach $MYDEV

    echo Compressing the disk image...
    hdiutil convert Riffle-$VERSION.dmg -format UDZO -o Riffle-$VERSION-compressed.dmg

    echo Internet enabling the disk image...
    hdiutil internet-enable Riffle-$VERSION-compressed.dmg

    echo Renaming compressed image...
    rm -f Riffle-$VERSION.dmg
    mv Riffle-$VERSION-compressed.dmg Riffle-$VERSION.dmg
}

# Build Riffle

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
    echo "Makes tarballs and optionally rebuilds Riffle."
    echo "Usage: $PROGNAME [ options ]"
    echo Options:
    echo "    --help               Display this help message"
    echo "    --build              Cleans and builds Riffle first"
    echo "    --debug-build        Cleans and debug-builds Riffle first"
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
	--install) INSTALLAPP=1 ;;
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
  echo Creating Version $VERSION distribution in $DESTDIR, using source directory $SRCDIR and riffle binary in $APPBUILDDIR.

  # Remove all existing files
  if [ ! -d "$DESTDIR/riffle" ]; then
      rm -f -r $DESTDIR/riffle
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
    echo Location of riffle binary $APPBUILDDIR not found.
    exit 1
fi

if [ ! -f $APPBINARY ]; then
  echo "*** Sorry, Riffle was not built correctly."
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

if [ "$INSTALLAPP" = "1" ]; then
    install_app
fi

echo Riffle archived.
