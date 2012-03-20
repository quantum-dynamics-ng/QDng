#!/usr/bin/perl


if ($#ARGV == -1){
  print STDOUT "No Inputfile given!\n\n";
  print STDOUT "Usage: codelet.pl template parameters\n\n";
  exit;
}

$codname = $ARGV[0];
$defname = $ARGV[1];

open DEF, $defname or die "Input file not found!\n";
open COD, $codname or die "Input file not found!\n";

$varcount = 0;


while($line = <DEF>) {
    chomp $line;
    if($line =~ /VAR/) {
      @entries = split(/\s+/,$line);
      $var{$entries[2]} = $entries[1];
      $vartype[$varcount] = $entries[1];
      $varname[$varcount] = $entries[2];
      printf "vardef %s %s\n", $entries[1],$entries[2] ;
      $varcount++;
    }
    if($line =~ /EXP/) {
      $line =~ s/^EXP\s+//;
      $exp = $line;
    }
    if($line =~ /NAME/) {
      $line =~ s/^NAME\s+//;
      $funcname = $line;
      printf "%s\n", $funcname;
    }
   
}    

{
  local $/=undef;
  $cod = <COD>;
  $/ = '\n';
}

$cod =~ s/\@DESC\@/$exp/g;

for ($i = 0; $i < $varcount; $i++) {
  if ($i > 0) {
    $params = sprintf("%s, ", $params);    
  }
  $params = sprintf "%s%s %s", $params, $vartype[$i], $varname[$i];
  
  if($vartype[$i] =~ /Vec/ ){
     $exp =~ s/$varname[$i]/($varname[$i]->begin(s))[i]/g;
  } else {
     $exp = $exp;
  }
}

$defname =~ s/\.pm//; 

printf "%s.\n", $defname;
printf "%s.\n", $funcname;
printf "%s.\n", $params;
printf "%s.\n", $exp;

$cod =~ s/\@NAME\@/$funcname/g;
$cod =~ s/\@PARAMS\@/$params/g;
$cod =~ s/\@EXP\@/$exp/;

open(HEADER, ">$defname.h");
printf HEADER "%s\n", $cod;
close(HEADER);

