#ifdef SCPU_CPP

void sCPU::op_io() {
  status.clock_count = 6;
  dma_edge();
  add_clocks(6);
  alu_edge();
}

uint8 sCPU::op_read(uint32 addr) {
  status.clock_count = speed(addr);
  dma_edge();
  add_clocks(status.clock_count - 4);
  regs.mdr = bus.read(addr);
  add_clocks(4);
  alu_edge();
  return regs.mdr;
}

void sCPU::op_write(uint32 addr, uint8 data) {
  alu_edge();
  status.clock_count = speed(addr);
  dma_edge();
  add_clocks(status.clock_count);
  if (addr == 0xfffe) {
    putc(data, stdout);
  }
  else if (addr == 0xfffd) {
    exit(data);
  }
  bus.write(addr, regs.mdr = data);
}

unsigned sCPU::speed(unsigned addr) const {
  if(addr & 0x408000) {
    if(addr & 0x800000) return status.rom_speed;
    return 8;
  }
  if((addr + 0x6000) & 0x4000) return 8;
  if((addr - 0x4000) & 0x7e00) return 6;
  return 12;
}

#endif
