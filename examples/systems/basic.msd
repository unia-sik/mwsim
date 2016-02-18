# $Id: basic.msd 693 2014-06-23 16:14:21Z klugeflo $
# Basic system description

system
  # assume 32 bit processor
  wordsize = 4

  #loadlatency = 10
  load_memory_latency = 32
  load_bus_latency = 2
  load_loop_init_latency = 76
  load_loop_exec_latency = 13

  #sendinitlatency = 5
  #sendlatency = 5
  send_loop_init_latency = 76
  send_loop_exec_latency = 13

  recv_loop_init_latency = 76
  recv_loop_exec_latency = 13
  #recvinitlatency =
  #recvlatency =

  tx_buffer_size = 512
  # these depend on the actual NoC
  #tx_period
  #tx_wctt
end system
