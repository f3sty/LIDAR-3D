#!/usr/bin/perl
use Math::Trig;
use Math::Trig::Degree
  qw(dsin dcos tan sec csc cot asin acos atan asec acsc acot sinh cosh tanh sech csch coth asinh acosh atanh asech acsch acoth);

my %azs;
my $plyhdr =
"ply\nformat ascii 1.0\nelement vertex COUNT\nproperty float x\nproperty float y\nproperty float z\nend_header\n";
my $count;
my @data;

my $outf = $ARGV[0];
$outf =~ s/\.\w{2,4}$/.ply/;
unless ( $outf =~ m/\.ply$/ ) { $outf .= ".ply"; }

open my $file, "<", $ARGV[0] or die "Failed to read input file\n";
while (<$file>) {
    next if (/#/);
    next unless ( $_ =~ m/\,/ );
    $count++;
    chomp;
    my ( $az, $inc, $dst ) = split /,/, $_;
    $azs{$az}++;
    $az  = 360 - $az;
    $inc = 90 - $inc;
    my $arad = deg2rad($az);
    my $irad = deg2rad($inc);
    my $x    = $dst * sin($irad) * cos($arad);
    my $y    = $dst * sin($irad) * sin($arad);
    my $z    = $dst * cos($irad);

    push( @data, "$x $y $z\n" );
}

open my $out, ">", $outf;
$plyhdr =~ s/COUNT/$count/;
print $out $plyhdr;
foreach my $dpoint (@data) {
    print $out $dpoint;
}
close $out;

foreach my $az_ ( keys %azs ) {
    print STDERR "$az_: $azs{$az_}\n";
}

