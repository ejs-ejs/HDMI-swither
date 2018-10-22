void CMD_output_name() {
  char *arg;
   arg = sCmd.next();
   if (arg != NULL) {
    strcpy(outputs[cOutput].Name, arg);
   }
   state_report();
  }
