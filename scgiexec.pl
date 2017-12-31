#!/usr/bin/env perl
use 5.012;

my %params = @ARGV;
my $root = $params{DOCUMENT_ROOT} or die "no DOCUMENT_ROOT";
my $file = $params{DOCUMENT_URI}  or die "no DOCUMENT_URI";
my $path = "$root$file";

chdir "$root" or die "chdir $root: $!";
exec $path $file, @ARGV;
die "exec $path: $!";
