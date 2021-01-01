#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // TSV file
  FILE  *fpandemic = NULL;
  // BIN files
  FILE  *fongoing = NULL,
        *fresolved = NULL,
        *fincomplete = NULL;

  int numLines = 0;
  unsigned char tempChar;

  char header[183];
  char *dataPiece;

  // Open the pandemic file for reading only
  fpandemic = fopen("pandemic.tsv", "r");
  if (!fpandemic) {
     printf("ERROR: Could not open pandemic file\n");
     exit(1);
  }

  // Open a new ongoing file for writing only
  fongoing = fopen("ongoing.bin", "w");
  if (!fongoing) {
     printf("ERROR: Could not open ongoing file\n");
     exit(1);
  }

  // Open a new resolved file for writing only
  fresolved = fopen("resolved.bin", "w");
  if (!fresolved) {
     printf("ERROR: Could not open resolved file\n");
     exit(1);
  }

  // Open a new incomplete file for writing only
  fincomplete = fopen("incomplete.bin", "w");
  if (!fincomplete) {
     printf("ERROR: Could not open incomplete file\n");
     exit(1);
  }

  // Read number of lines in the file
  while(!feof(fpandemic)){
    tempChar = fgetc(fpandemic);
    if (tempChar == '\n'){
      numLines++;
    }
  }
  printf("The number of lines in pandemic.tsv is %d\n", numLines);

  //Discard the header
  fscanf(fpandemic, "%[^\t]s", (char)header);
  printf("Header: %s\n", header);


  // Sort info into the binary files

  /*
  For part 7 of the assignment it suggests we create a helper
  function to convert some of the character strings to ints,
  are we allowed to make us of the atoi() function within our
  helper function for this? I don't believe we've covered it
  in class so far, so I just wanted to clarify if this was
  acceptable.
  */

  // ONGOING - #cases > #deaths + #recovered
  // RESOLVED - #cases = #deaths + #recovered
  // INCOMPLETE - missing data in total cases, total deaths, total recovered or population

  // fscanf(fpandemic, "%[^\t]s", readTsv);


  // All done ... close the files
  fclose(fpandemic);
  fclose(fongoing);
  fclose(fresolved);
  fclose(fincomplete);

  printf("All files are closed\n");
  return 0;
}
