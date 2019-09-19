static void
printmap (void)
{
  char name[64];
  char line[64];
  FILE *f;

  sprintf (name, "/proc/%d/maps", getpid ());
  f = fopen (name, "r");
  if (!f)
    return;
  while (fgets (line, sizeof (line), f))
    fprintf (stderr, line);
  fclose (f);
}
