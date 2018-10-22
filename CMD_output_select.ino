void CMD_output_select() {
   int aNumber;
  char *arg;
   arg = sCmd.next();
   if (arg != NULL) {
      aNumber = atol(arg);
      cOutput = (aNumber <= NUM_CMDS)? aNumber-1: cOutput;
   }
   state_report();
  }
