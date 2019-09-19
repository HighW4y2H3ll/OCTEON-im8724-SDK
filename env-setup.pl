#!/usr/bin/perl

use strict;
use Getopt::Long;
use Cwd;
use Env qw(PATH __OCTEON_SHELL_TEST__);


# Determine if we should use setenv or export for variables.
# The shell name that is passed as an argument is unreliable,
# sometimes just contains '-su'
my $use_setenv = 0;
$use_setenv = 1 if ($__OCTEON_SHELL_TEST__ == 2);

# Clean up empty args we get from wrapper script
my $tmp;
while (!$tmp && scalar(@ARGV))
{
   $tmp = pop(@ARGV);
   push(@ARGV, $tmp) if $tmp;
}

my $octeon_model;
my $octeon_root;

# process command line options
my $runtime_model_flag = 1;
my $verbose = 0;
my $enable_checks_flag = 0;
my $little_endian_flag = 0;
my $liquidio_flag = 0;
GetOptions("runtime-model!"      => \$runtime_model_flag,
           "verbose!"            => \$verbose,
           "checks!"             => \$enable_checks_flag,
           "little-endian!"      => \$little_endian_flag,
           "liquidio!"           => \$liquidio_flag,
           );


if (scalar(@ARGV) > 1 || scalar(@ARGV) < 1)
{
   usage();
   print 'echo ""';
   exit;
}
elsif (scalar(@ARGV) == 2)
{
   $octeon_root = $ARGV[1];
}
else
{
   $octeon_root = cwd();
}
$octeon_model = $ARGV[0];


sub usage
{
   warn "Usage: source ./env-setup <OCTEON_MODEL> [--runtime-model] [--checks] ...\n";
   warn " OCTEON_MODEL:     Model of OCTEON to build and simulate for.\n";
   warn " --runtime-model:  Enables runtime model detection build option by setting environment variable.\n";
   warn "                   Use --noruntime-model to clear environment variable if desired.\n";
   warn "                   Default is --runtime-model.\n";
   warn " --checks:         Enables various consistancy and programming checks in CVMX.\n";
   warn "                   Use --nochecks to clear environment variable if desired.\n";
   warn "                   Default is --nochecks.\n";
   warn " --little-endian:  Uses little endian toolchain for building applications.\n";
   warn " --verbose:        Be verbose about what the script is doing\n";
   warn "                   Default is --noverbose.\n";
   warn "\n";
   warn "The env-setup script must be invoked from the root directory of the SDK install,\n";
   warn "as it sets OCTEON_ROOT to be the directory that it is invoked in.\n";
   warn "\n";
}


my %env_hash;  # Hash to fill with env variables to set
my @env_clear; #list of environment variables to clear
my $key;
my $extra_path = "$octeon_root/tools/bin:$octeon_root/tools-le/bin:$octeon_root/host/bin";


# validate OCTEON_MODEL.  Warn but proceed if if octeon-models.txt is not present.
if (open(FH, "$octeon_root/octeon-models.txt"))
{
   my @models = <FH>;
   my ($match) = grep(/^$octeon_model$/, @models);
   if (!$match)
   {
      warn "ERROR: $octeon_model is not a valid OCTEON_MODEL value.  Please see \$OCTEON_ROOT/octeon-models.txt for list\n";
      print "echo \"`column $octeon_root/octeon-models.txt`\"";
      exit;
   }
}
else
{
   warn 'Warning: unable to open file $OCTEON_ROOT/octeon-models.txt, can\'t validate OCTEON_MODEL\n';
}



# Set up hash of environment variables to set
$env_hash{'OCTEON_MODEL'} = $octeon_model;
$env_hash{'OCTEON_ROOT'} = $octeon_root;
if (!($PATH=~ /^$extra_path/))
{
   $env_hash{'PATH'} = $extra_path.':$PATH';
   warn "Updating PATH, adding $extra_path to beginning.\n" if ($verbose);
}
else
{
   warn "Not updating PATH - OCTEON SDK dirs already present.\n" if ($verbose);
}

my $global_add = "";

if ($runtime_model_flag)
{
   $global_add .= ' -DUSE_RUNTIME_MODEL_CHECKS=1';
}

if ($enable_checks_flag)
{
   $global_add .= ' -DCVMX_ENABLE_PARAMETER_CHECKING=1 -DCVMX_ENABLE_CSR_ADDRESS_CHECKING=1 -DCVMX_ENABLE_POW_CHECKS=1';
}
else
{
   $global_add .= ' -DCVMX_ENABLE_PARAMETER_CHECKING=0 -DCVMX_ENABLE_CSR_ADDRESS_CHECKING=0 -DCVMX_ENABLE_POW_CHECKS=0';
}

if ($global_add)
{
   $env_hash{'OCTEON_CPPFLAGS_GLOBAL_ADD'} = $global_add;
}
else
{
   push(@env_clear, 'OCTEON_CPPFLAGS_GLOBAL_ADD');
}

if ($little_endian_flag)
{
   $env_hash{'OCTEON_LE'} = $little_endian_flag;
}
else
{
    push(@env_clear, 'OCTEON_LE');
}

if ($liquidio_flag)
{
   $env_hash{'OCTEON_LIQUIDIO'} = 1
}
else
{
    push(@env_clear, 'OCTEON_LIQUIDIO');
}

#print out what we are doing
if ($verbose)
{
   foreach $key (keys(%env_hash))
   {
      warn "Setting $key  to ".'"'."$env_hash{$key}".'"'."\n";
   }
   if (@env_clear)
   {
      foreach $key (@env_clear)
      {
         warn "Unsetting  $key\n";
      }
   }
}


# Generate environment setting command based on shell
my $env_cmd;
if ($use_setenv)  # csh and tcsh
{
   foreach $key (keys(%env_hash))
   {
      $env_cmd .= "setenv $key ".'"'."$env_hash{$key}".'"'.';';
   }
   if (@env_clear)
   {
      foreach $key (@env_clear)
      {

         $env_cmd .= ";unsetenv  $key ";
      }
   }
   $env_cmd .= "\n";
   warn "CSH command: $env_cmd\n" if ($verbose);
   print $env_cmd;
}
else
{
   $env_cmd = 'export ';
   foreach $key (keys(%env_hash))
   {

      $env_cmd .= "$key=".'"'."$env_hash{$key}".'"'.' ';
   }
   if (@env_clear)
   {
      $env_cmd .= ";export ";
      foreach $key (@env_clear)
      {
         # ksh doesn't support -n, so we just clear the variable
         $env_cmd .= "$key= ";
      }
   }

   $env_cmd .= "\n";
   warn "SH command: $env_cmd\n" if ($verbose);
   print $env_cmd;
}
