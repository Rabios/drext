$gtk.ffi_misc.gtk_dlopen("drclip")
include FFI::CExt

def tick args
  init_state = drclip_init
  
  if init_state == 0
    raise "ERROR: Could not initialize Clipboard!"
  end

  args.outputs.primitives << {
    x: 0,
    y: 100.from_top,
    w: 1280,
    h: 100
  }.to_solid
  
  args.outputs.primitives << {
    x: 10,
    y: 40.from_top,
    x2: 1270,
    y2: 40.from_top,
    r: 255,
    g: 0,
    b: 255,
    a: 255
  }.to_line
  
  args.outputs.primitives << {
    x: 10,
    y: 10.from_top,
    text: "Press SPACE key to Copy sample text! (Copied text appears below)",
    r: 255,
    g: 0,
    b: 255,
    a: 255
  }.to_label
  
  args.outputs.primitives << {
    x: 10,
    y: 60.from_top,
    text: drclip_get.str,
    r: 255,
    g: 255,
    b: 255,
    a: 255
  }.to_label
  
  if args.inputs.keyboard.key_down.space
    drclip_set "COPY!"
  end
  
  if init_state == 1
    drclip_uninit
  end
end
