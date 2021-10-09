$gtk.ffi_misc.gtk_dlopen("drcpu")
include FFI::CExt

def tick args
  info = drcpu
  
  args.outputs.primitives << {
    x: 0,
    y: 100.from_top,
    w: 600,
    h: 100,
    r: 0,
    g: 0,
    b: 0,
    a: 255
  }.to_solid
  
  args.outputs.primitives << {
    x: 10,
    y: 10.from_top,
    text: "CPU Cores: #{info.cores}",
    r: 255,
    g: 255,
    b: 255,
    a: 255
  }.to_label
  
  args.outputs.primitives << {
    x: 10,
    y: 40.from_top,
    text: "CPU Name: #{info.name.str}",
    r: 255,
    g: 255,
    b: 255,
    a: 255
  }.to_label
  
  drcpu_free(info)
end
