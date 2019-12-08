#!/bin/sh

# Make a distribution of an application.

# If your zip accepts Cygwin-style paths, then
# use cygpath, else substitute echo
CYGPATHPROG=cygpath
#CYGPATHPROG=echo

PROGNAME=$0

PROJECTDIR=c:/projects/AcmeApp
SCRIPTDIR=$PROJECTDIR/AcmeApp/scripts

# Read the variables
. $SCRIPTDIR/setup.var

# This script will set VERSION to the required version from symbols.h
VERSION=
RESOURCESONLY=0
NOBUILD=0

doreplace()
{
    thefile=$1
    theexpr=$2

    if [ -f $thefile ]; then
      sed -e "$theexpr" < $thefile > $thefile.tmp
      mv $thefile.tmp $thefile
    else
      echo "*** $thefile not found."
    fi
}

unix2dosname()
{
    echo $1 | sed -e "s/\//\\\\\\\/g" > /tmp/filename.tmp
    RETVALUE=`cat /tmp/filename.tmp`
    rm -f /tmp/filename.tmp
}

unix2dosname2()
{
    echo $1 | sed -e "s/\//\\\\/g" > /tmp/filename.tmp
    RETVALUE=`cat /tmp/filename.tmp`
    rm -f /tmp/filename.tmp
}

findversion()
{
	echo "#include <stdio.h>" > /tmp/appver.c
	echo "#include \"$VERSIONSYMBOLFILE\"" >> /tmp/appver.c
	echo "int main() { printf(\"%.2f\", $VERSIONSYMBOL); }" >> /tmp/appver.c
	gcc /tmp/appver.c -I$APPDIR -o /tmp/appver
    VERSION=`/tmp/appver`
	rm -f /tmp/appver /tmp/appver.c
}

make_resources_only()
{
    if [ -d $SETUPIMAGEDIR ]; then
        echo Removing existing $SETUPIMAGEDIR
        rm -f -r $SETUPIMAGEDIR
    fi

    mkdir -p $SETUPIMAGEDIR

    cd $RESOURCESDIR
    echo Compiling resource file $RESOURCESFILE
    zip -q $SETUPIMAGEDIR/$RESOURCESFILE *
}

makesetup()
{
    if [ -d $SETUPIMAGEDIR ]; then
        echo Removing existing $SETUPIMAGEDIR
        rm -f -r $SETUPIMAGEDIR
    fi

    echo Making the $SETUPIMAGEDIR for preparing the setup
    mkdir -p $SETUPIMAGEDIR

    mkdir -p $SETUPIMAGEDIR/Samples

    echo Copying sample files...
    cp -r $APPDIR/Samples $SETUPIMAGEDIR

    cd $SETUPIMAGEDIR

    # Copying readmes
    if [ "$READMEFILE" != "" ] && [ -f $READMEFILE ]; then
        echo Copying readme.txt
        cp $READMEFILE $SETUPIMAGEDIR
    fi

    if [ "$LICENSEFILE" != "" ] && [ -f $LICENSEFILE ]; then
        echo Copying license.txt
        cp $LICENSEFILE $SETUPIMAGEDIR
    fi

    if [ "$RESOURCESFILE" != "" ] && [ "$RESOURCESDIR" != "" ]; then
        if [ -d $RESOURCESDIR ]; then
            cd $RESOURCESDIR
            echo Compiling resource file $RESOURCESFILE
            zip -q $SETUPIMAGEDIR/$RESOURCESFILE * cookies/*
        else
            echo "*** Warning - $RESOURCESDIR directory not found"
        fi
    fi

    if [ "$MAKEMANUAL" != "0" ]; then
        if [ -d $MANUALDIR ]; then
            cd $MANUALDIR
            make
        else
            echo "*** Warning - $MANUALDIR not found"
        fi
    fi

    for EACHFILE in $MANUALFILES ; do
        if [ -f $APPDIR/manual/$EACHFILE ]; then
            echo Copying $EACHFILE
            cp $APPDIR/manual/$EACHFILE $SETUPIMAGEDIR
        else
            echo "*** Warning - $APPDIR/manual/$EACHFILE not found"
        fi
    done

    if [ -f $APPBINARY ]; then
        echo Copying binary $APPBINARY
        cp $APPBINARY $SETUPIMAGEDIR
    else
        echo "*** Error - $APPBINARY not found"
        exit 1
    fi

    ##### GENERATE THE SCRIPT #######

    echo Generating $SETUPSCRIPTNAME
    rm -f $SETUPSCRIPTNAME

    sh $SCRIPTDIR/makeinno.sh $SETUPIMAGEDIR $INNOTOP $INNOBOTTOM $SETUPSCRIPTNAME

    if [ ! -f $SETUPSCRIPTNAME ]; then
        echo "*** Error - something went wrong with the script file generation."
        exit 1
    fi

    # Now replace %VERSION% with the real application version, and other
    # variables
    echo Replacing variables in the setup script
    doreplace $SETUPSCRIPTNAME "s/%VERSION%/$VERSION/g"
    doreplace $SETUPSCRIPTNAME "s/%COPYRIGHTHOLDER%/$AUTHOR/g"
    doreplace $SETUPSCRIPTNAME "s/%VENDOR%/$VENDOR/g"

    unix2dosname $READMEFILE
    doreplace $SETUPSCRIPTNAME "s;%READMEFILE%;$RETVALUE;g"

    unix2dosname $READMEAFTERFILE
    doreplace $SETUPSCRIPTNAME "s;%READMEAFTERFILE%;$RETVALUE;g"

    unix2dosname $LICENSEFILE
    doreplace $SETUPSCRIPTNAME "s;%LICENSEFILE%;$RETVALUE;g"

    doreplace $SETUPSCRIPTNAME "s/%APPNAME%/$APPNAME/g"
    doreplace $SETUPSCRIPTNAME "s/%APPTITLE%/$APPTITLE/g"
    doreplace $SETUPSCRIPTNAME "s/%DEFAULTDIR%/$DEFAULTDIR/g"

    unix2dosname $SETUPIMAGEDIR
    doreplace $SETUPSCRIPTNAME "s;%SOURCEDIR%;$RETVALUE;g"

    unix2dosname $DESTDIR
    doreplace $SETUPSCRIPTNAME "s;%OUTPUTDIR%;$RETVALUE;g"

    doreplace $SETUPSCRIPTNAME "s/%APPEXTENSION%/$APPEXTENSION/g"

    if [ "$MANUALFILES" != "" ]; then
        HELPFILE=`basename $MANUALFILES`
        unix2dosname $HELPFILE
        doreplace $SETUPSCRIPTNAME "s;%HELPFILE%;$RETVALUE;g"
    fi

    rm -f $DESTDIR/setup*.*

    # Inno Setup complains if this step is not done
    unix2dos --unix2dos $SETUPSCRIPTNAME
    
    # Now invoke INNO compiler on the new ISS file
    # First, make a DOS filename or Inno Setup will get confused.

    unix2dosname2 $SETUPSCRIPTNAME
    DOSFILENAME=$RETVALUE

    # Note: the double slash is Mingw32/MSYS convention for
    # denoting a switch, that must not be converted into
    # a path (otherwise /c = c:/)

    cd `dirname $SETUPSCRIPTNAME`
    BASESCRIPTNAME=`basename $SETUPSCRIPTNAME`
    echo Invoking Inno Setup compiler on $BASESCRIPTNAME

    "$SETUPCOMPILER" //cc $BASESCRIPTNAME

    if [ ! -f $DESTDIR/setup.exe ]; then
        echo "*** Error - the setup.exe was not generated."
        exit
    fi

    cd $DESTDIR
    mv setup.exe $ARCHIVENAME-$VERSION-Setup.exe

    echo If you saw no warnings or errors, $APPTITLE was successfully spun.
    echo
}

usage()
{
    echo "Usage: $PROGNAME [ options ]" 1>&2
    echo Options:
    echo "    --help              Display this help message"
    echo "    --no-build          Do not build the setup.exe"
    echo "    --resources         Only make the resource file"
    echo.
    echo Note that options only override settings in $SCRIPTDIR/setup.var.
    exit 1
}

# Process command line options.

for i in "$@"; do
    case "$i" in
	--no-build) NOBUILD=1 ;;
	--resources) RESOURCESONLY=1 ;;
	*)
	    usage
	    exit
	    ;;
    esac
done

findversion

if [ "$RESOURCESONLY" = "1" ]; then
    make_resources_only
    exit    
fi

if [ "$NOBUILD" = "0" ]; then
    makesetup
fi
