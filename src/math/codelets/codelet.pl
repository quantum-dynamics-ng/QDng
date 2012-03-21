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
$sse_exp = "";

while($line = <DEF>) {
    chomp $line;
    if($line =~ /^VAR/) {
      @entries = split(/\s+/,$line);
      $var{$entries[2]} = $entries[1];
      $vartype[$varcount] = $entries[1];
      $varname[$varcount] = $entries[2];
      printf "vardef %s %s\n", $entries[1],$entries[2] ;
      $varcount++;
    }
    if($line =~ /^EXP/) {
      $line =~ s/^EXP\s+//;
      $exp = $line;
    }
    if($line =~ /^SSE_EXP/) {
      $line =~ s/^SSE_EXP\s+//;
      $sse_exp = $line;
    }
    if($line =~ /^NAME/) {
      $line =~ s/^NAME\s+//;
      $funcname = $line;
      printf "Name: %s\n", $funcname;
    }
   
}    

{
  local $/=undef;
  $cod = <COD>;
  $/ = '\n';
}

$cod =~ s/\@DESC\@/$exp/g;

printf "Expression: ";
printf "%s\n",$exp;

# default SSE expression is dervied from skalar
if (length($sse_exp) == 0){
  $sse_exp = $exp;
} else {
  printf "SSE Expression: %s\n", $sse_exp;
}

# Replace lhs of SSE with result dummy varcount
@entries = split(/\s+/,$sse_exp);
$sse_lhs = @entries[$0];
$sse_exp =~  s/^$sse_lhs\s+=/_res =/;


#
# Replace Variables
#
for ($i = 0; $i < $varcount; $i++) {
  if ($i > 0) {
    $params = sprintf("%s, ", $params);    
  }

  $params = sprintf "%s%s %s", $params, $vartype[$i], $varname[$i];
  if ($vartype[$i] =~ /Vec/ ){
     $exp =~ s/$varname[$i]/($varname[$i]->begin(s))[i]/g;
  }
  
  # SSE specific
  if ($vartype[$i] =~ /cVec/ ){
     $sse_exp =~ s/$varname[$i]/m128dc(($varname[$i]->begin(s))[i])/g;
     if ($varname[$i] eq $sse_lhs){
        $sse_exp = "m128dc " . $sse_exp;
        $lhs_type = "cVec";
     }
  }
  if ($vartype[$i] =~ /dVec/ ){
     $sse_exp =~ s/$varname[$i]/($varname[$i]->begin(s))[i]/g;
     if ($varname[$i] eq $sse_lhs){
        $sse_exp = "double " . $sse_exp;
        $lhs_type = "dVec";
     }
  }
  if ($vartype[$i] =~ /dcomplex/ ){
     $sse_exp =~ s/ $varname[$i]/ m128dc($varname[$i]) /g;
  }

}

# Reassign SSE result to scalar types
if ($lhs_type =~ /cVec/){
   $sse_exp = sprintf "%s; _res.Store((%s->begin(s))[i]);", $sse_exp, $sse_lhs;
} elsif ($lhs_type =~ /dVec/) {
  $sse_exp = sprintf "%s; (%s->begin(s))[i] = _res;", $sse_exp, $sse_lhs;
}else {
   $sse_exp = sprintf "%s; %s = _res;", $sse_exp, $sse_lhs;
}


$exp = $exp . ";";

$defname =~ s/\.pm//; 

printf "%s\n", $defname;
printf "%s\n", $funcname;
printf "%s\n", $params;
printf "Scalar:\t %s\n", $exp;
printf "SSE2:\t %s\n", $sse_exp;

$cod =~ s/\@NAME\@/$funcname/g;
$cod =~ s/\@PARAMS\@/$params/g;
$cod =~ s/\@EXP\@/$exp/;
$cod =~ s/\@SSE_EXP\@/$sse_exp/;

open(HEADER, ">$defname.h");
printf HEADER "%s\n", $cod;
close(HEADER);

