#/usr/bin/perl

use strict;
use warnings;

die "perl $0 <FI:data> <FI:pond> <INT:top>\n" unless($#ARGV==2);
my ($file_data,$file_pond,$top)=@ARGV;

my %data=();
my $score=0;
my @S=();
open IN,$file_pond or die "$0: $file_pond: $!\n";
while(<IN>){
	chomp;
	if(/^score=(.*?),/){
		$score=$1;#abs($1);
	}elsif(/^position=(.*)/){
		#my @F=split /,/,$1;
		if(scalar(@S)<=$top+1 || $score>=$S[$top-1]){
			$data{$1}=$score;
			push @S,$score;
			@S=sort{$b<=>$a} @S;
		}
	}
}
close IN;

my @score=values %data;
@score=sort{$b<=>$a}@score;

$top=$#score if($top>$#score);
my $cutoff=$score[$top];
#print STDERR "cutoff=$cutoff\n";

my @k =sort{$data{$b}<=>$data{$a}} keys %data;

open IN,$file_data or die "$0: $file_data: $!\n";
while(<IN>){
	chomp;
	my @F=split /,/;
	my $n=0;
	for my $p (@k){
		next if($data{$p}<$cutoff);
		my @L=split /,/,$p;
		print "$n";
		print "\t$F[$_]" for(@L);
		print "\n";
		$n++;
	}
}
close IN;
