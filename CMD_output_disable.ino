void CMD_output_disable() {
  outputs[cOutput].Active = 0;
  state_report();
  }
