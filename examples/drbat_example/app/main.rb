$gtk.ffi_misc.gtk_dlopen("drbat")
include FFI::CExt

def tick args
  status = drbat
  
  args.outputs.primitives << {
    x: 0,
    y: 120.from_top,
    w: 400,
    h: 120,
    r: 0,
    g: 0,
    b: 0,
    a: 255
  }.to_solid
  
  args.outputs.primitives << {
    x: 10,
    y: 10.from_top,
    text: "Charging: #{status.is_charging == 1 ? "YES" : "NO"}",
    r: 255,
    g: 255,
    b: 255,
    a: 255
  }.to_label
  
  args.outputs.primitives << {
    x: 10,
    y: 40.from_top,
    text: "Battery Level: %#{status.percent}",
    r: 255,
    g: 255,
    b: 255,
    a: 255
  }.to_label
  
  args.outputs.primitives << {
    x: 10,
    y: 70.from_top,
    text: "Battery Lifetime: #{status.life_time} seconds",
    r: 255,
    g: 255,
    b: 255,
    a: 255
  }.to_label
end
