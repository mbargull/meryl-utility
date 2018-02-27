#!/usr/bin/env perl

###############################################################################
 #
 #  This file is part of canu, a software program that assembles whole-genome
 #  sequencing reads into contigs.
 #
 #  This software is based on:
 #    'Celera Assembler' (http://wgs-assembler.sourceforge.net)
 #    the 'kmer package' (http://kmer.sourceforge.net)
 #  both originally distributed by Applera Corporation under the GNU General
 #  Public License, version 2.
 #
 #  Canu branched from Celera Assembler at its revision 4587.
 #  Canu branched from the kmer project at its revision 1994.
 #
 #  Modifications by:
 #
 #    Brian P. Walenz beginning on 2016-JAN-11
 #      are a 'United States Government Work', and
 #      are released in the public domain
 #
 #  File 'README.licenses' in the root directory of this distribution contains
 #  full conditions and disclaimers for each license.
 ##

use strict;
use File::Compare;
use Cwd qw(getcwd);

my $cwd = getcwd();

my $label    = "snapshot";     #  Automagically set to 'release' for releases.
my $major    = "1";            #  Bump before release.
my $minor    = "7";            #  Bump before release.

my $commits  = "0";
my $hash1    = undef;          #  This from 'git describe'
my $hash2    = undef;          #  This from 'git rev-list'
my $revCount = 0;
my $dirty    = undef;


#  If in a git repo, we can get the actual values.

if (-d "../.git") {
    $label = "snapshot";

    open(F, "git rev-list HEAD |") or die "Failed to run 'git rev-list'.\n";
    while (<F>) {
        chomp;

        $hash2 = $_  if (!defined($hash2));
        $revCount++;
    }
    close(F);

    open(F, "git describe --tags --long --dirty --always --abbrev=40 |") or die "Failed to run 'git describe'.\n";
    while (<F>) {
        chomp;
        if (m/^v(\d+)\.(\d+.*)-(\d+)-g(.{40})-*(.*)$/) {
            $major   = $1;
            $minor   = $2;
            $commits = $3;
            $hash1   = $4;
            $dirty   = $5;
        } else {
            die "Failed to parse describe string '$_'.\n";
        }
    }
    close(F);

    if ($dirty eq "dirty") {
        $dirty = "ahead of github";
    } else {
        $dirty = "sync'd with guthub";
    }
}


#  If not in a git repo, we might be able to figure things out based on the directory name.

elsif ($cwd =~ m/canu-(.{40})\/src/) {
    $label    = "snapshot";
    $hash1    = $1;
    $hash2    = $1;
}

elsif ($cwd =~ m/canu-master\/src/) {
    $label    = "master-snapshot";
}

elsif ($cwd =~ m/canu-(\d).(\d)\/src/) {
    $label    = "release";
    $major    = $1;
    $minor    = $2;
}



#  Report what we found.  This is really for the gmake output.

if ($commits > 0) {
    print STDERR "Building snapshot v$major.$minor +$commits changes (r$revCount $hash1) ($dirty)\n";
    print STDERR "\n";
} else {
    print STDERR "Building $label v$major.$minor\n";
    print STDERR "\n";
}

#  Dump a new file, but don't overwrite the original.

open(F, "> canu_version.H.new") or die "Failed to open 'canu-version.H.new' for writing: $!\n";
print F "//  Automagically generated by canu_version_update.pl!  Do not commit!\n";
print F "#define CANU_VERSION_LABEL     \"$label\"\n";
print F "#define CANU_VERSION_MAJOR     \"$major\"\n";
print F "#define CANU_VERSION_MINOR     \"$minor\"\n";
print F "#define CANU_VERSION_COMMITS   \"$commits\"\n";
print F "#define CANU_VERSION_REVISION  \"$revCount\"\n";
print F "#define CANU_VERSION_HASH      \"$hash1\"\n";

if      (defined($dirty)) {
    print F "#define CANU_VERSION           \"Canu snapshot v$major.$minor +$commits changes (r$revCount $hash1)\\n\"\n";
} elsif (defined($hash1)) {
    print F "#define CANU_VERSION           \"Canu snapshot ($hash1)\\n\"\n";
} else {
    print F "#define CANU_VERSION           \"Canu $major.$minor\\n\"\n";
}

close(F);

#  If they're the same, don't replace the original.

if (compare("canu_version.H", "canu_version.H.new") == 0) {
    unlink "canu_version.H.new";
} else {
    unlink "canu_version.H";
    rename "canu_version.H.new", "canu_version.H";
}

#  That's all, folks!

exit(0);

