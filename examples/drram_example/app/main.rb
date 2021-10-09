$gtk.ffi_misc.gtk_dlopen("drram")
include FFI::CExt

def tick args
  info = drram
  
  args.outputs.primitives << {
    x: 0,
    y: 100.from_top,
    w: 400,
    h: 100,
    r: 0,
    g: 0,
    b: 0,
    a: 255
  }.to_solid
  
  args.outputs.primitives << {
    x: 10,
    y: 10.from_top,
    text: "Total RAM: #{(info.total / (1024 * 1024 * 1024))} GB",
    r: 255,
    g: 255,
    b: 255,
    a: 255
  }.to_label
  
  args.outputs.primitives << {
    x: 10,
    y: 40.from_top,
    text: "Free RAM: #{(info.free / (1024 * 1024 * 1024))} GB",
    r: 255,
    g: 255,
    b: 255,
    a: 255
  }.to_label
end
