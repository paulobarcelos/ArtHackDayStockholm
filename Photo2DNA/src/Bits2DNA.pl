# Original code extracted from paper http://www.sciencemag.org/content/suppl/2012/08/15/science.1226355.DC1/Church.SM.pdf
	
# \Perl\bin\perl Bits2DNA.pl GMC Jul-2011 & 27-May-2012
# docstore.mik.ua/orelly/perl/cookbook/ch02_05.htm (bin) ch01_05.htm (char)
# http://perldoc.perl.org/functions/pack.html rand.html
# Each oligo is L(19)+8N(12)= 115 bp, long flanked by 22-mer amplification primers.
# DNA Encoded Artifacts Registry (DEAR) to coordinate global standards.
open IN,$ARGV[0]; open OUT,">$ARGV[0].dna"; binmode IN;
$t{"0"}="a"; $t{"1"}="G"; # lowercase a,c = zero bit.
$t{"a"}="c"; $t{"G"}="T"; $t{"c"}="a"; $t{"T"}="G";
$u1=""; $u2=""; $u3=""; # Initialize; keep homopolymer runs < 4
$N=12; # Length of segment in bytes (not including segment number)
$L=19; # 2^19 = 524,288 = max number of oligos L=00010011
$seed=2; srand($seed); # remove this line to get a random seed
$f="CTACACGACGCTCTTCCGATCT"; # forward 'universal' sequencing & amplification primer
$r="AGATCGGAAGAGCGGTTCAGCA"; # reverse 22-mer primer
$n=0; print OUT $f,int2bp(0),""; ###
while (read (IN, $text, 65536)) {
	@ascii_num = unpack("C*", $text);
	foreach $val (@ascii_num) {
		print OUT byt2bp($val); ###
		$n++;
		if($n%$N==0){
			print OUT $r,"\n",$f,int2bp($n/$N),""; ###
		} # N bases per output line
	} # each byte
} # 65 Kbytes
for ($k=$n%$N; $k<$N; $k++){
	print OUT byt2bp(int(rand(256))); ###
} # pad last data line to keep all oligos same size.
print OUT "$r\n"; ###
sub byt2bp { # convert rightmost 8 bits (MSB first byte) to 8 bp
	my $b = unpack("B32", pack("N", shift));
	$p="";
	for ($i=24; $i<=31; $i++){
		$x=substr($b,$i,1); # bits 24 to 31 inclusive
		$u=$t{$x};
		if(rand(2)<1){$u=$t{$u};} # pick synonym a=c; G=T
		if(($u eq $u1) && ($u eq $u2) && ($u eq $u3)){$u=$t{$u};}
		$u1=$u2; $u2=$u3; $u3=$u; # Shift previous base string
		$p = $p.$u;
	}
	return $p;
}
sub int2bp { # convert rightmost $L bits of 32 bit integers to $L bp
	my $b = unpack("B32", pack("N", shift));
	$p="";
	for ($i=31; $i>=32-$L; $i--){
		$x=substr($b,$i,1); # bits 31 to $L
		$u=$t{$x};
		if(rand(2)<1){$u=$t{$u};} # pick synonym a=c; G=T
		if(($u eq $u1) && ($u eq $u2) && ($u eq $u3)){$u=$t{$u};}
		$u1=$u2; $u2=$u3; $u3=$u; # Shift previous base string
		$p = $p.$u;
	}
	return $p;
}