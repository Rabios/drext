#include <mruby.h>
#include <string.h>
#include <assert.h>
#include <mruby/string.h>
#include <mruby/data.h>
#include <dragonruby.h>

#include <mach/mach.h>
#include <mach/mach_host.h>

typedef struct drram_stats {
  unsigned long long int total;
  unsigned long long int free;
} drram_stats;

extern drram_stats drram(void);

extern drram_stats drram(void) {
    mach_port_t host_port;
    mach_msg_type_number_t host_size;
    vm_size_t pagesize;
    vm_statistics_data_t vm_stat;
    
    host_port = mach_host_self();
    host_size = sizeof(vm_statistics_data_t) / sizeof(integer_t);
    host_page_size(host_port, &pagesize);

    if (host_statistics(host_port, HOST_VM_INFO, (host_info_t)&vm_stat, &host_size) != KERN_SUCCESS) {
        return (drram_stats) { 0, 0 };
    }
    
    natural_t mem_used = (vm_stat.active_count + vm_stat.inactive_count + vm_stat.wire_count) * pagesize;
    natural_t mem_free = vm_stat.free_count * pagesize;
    natural_t mem_total = mem_used + mem_free;
    
    return (drram_stats) {
        mem_total,
        mem_free
    };
}

// MRuby `typedef`s mrb_int in the mruby/value.h
// Then `#define`s mrb_int in mruby.h
// We need to undo the macro and avoid it's usage
// FIXME: I'm surely doing something wrong
#ifdef mrb_int
#undef mrb_int
#endif

void *(*drb_symbol_lookup)(const char *sym) = NULL;

static void (*drb_free_foreign_object_f)(mrb_state *, void *);
static void (*drb_typecheck_float_f)(mrb_state *, mrb_value);
static void (*drb_typecheck_int_f)(mrb_state *, mrb_value);
static void (*drb_typecheck_bool_f)(mrb_state *, mrb_value);
static void (*drb_typecheck_aggregate_f)(mrb_state *, mrb_value, struct RClass *, mrb_data_type *);
static struct RClass *(*mrb_module_get_f)(mrb_state *, const char *);
static mrb_int (*mrb_get_args_f)(mrb_state *, mrb_args_format, ...);
static struct RClass *(*mrb_module_get_under_f)(mrb_state *, struct RClass *, const char *);
static struct RClass *(*mrb_class_get_under_f)(mrb_state *, struct RClass *, const char *);
static struct RClass *(*mrb_define_module_under_f)(mrb_state *, struct RClass *, const char *);
static void (*mrb_define_module_function_f)(mrb_state *, struct RClass *, const char *, mrb_func_t, mrb_aspec);
static struct RClass *(*mrb_define_class_under_f)(mrb_state *, struct RClass *, const char *, struct RClass *);
static void (*mrb_define_method_f)(mrb_state *, struct RClass *, const char *, mrb_func_t, mrb_aspec);
static void (*mrb_define_class_method_f)(mrb_state *, struct RClass *, const char *, mrb_func_t, mrb_aspec);
static struct RData *(*mrb_data_object_alloc_f)(mrb_state *, struct RClass *, void *, const mrb_data_type *);
static mrb_value (*mrb_str_new_cstr_f)(mrb_state *, const char *);
static void (*mrb_raise_f)(mrb_state *, struct RClass *, const char *);
static struct RClass *(*mrb_exc_get_f)(mrb_state *, const char *);
static void drb_free_foreign_object_indirect(mrb_state *state, void *pointer) {
    drb_free_foreign_object_f(state, pointer);
}
static int drb_ffi__ZTSi_FromRuby(mrb_state *state, mrb_value self) {
    drb_typecheck_int_f(state, self);
    return mrb_fixnum(self);
}
static mrb_value drb_ffi__ZTSi_ToRuby(mrb_state *state, int value) {
    return mrb_fixnum_value(value);
}
struct drb_foreign_object_ZTS9drram_stats {
    drb_foreign_object_kind kind;
    drram_stats value;
};
static mrb_data_type ForeignObjectType_ZTS9drram_stats = {"drram_stats", drb_free_foreign_object_indirect};
static drram_stats drb_ffi__ZTS9drram_stats_FromRuby(mrb_state *state, mrb_value self) {
    struct RClass *FFI = mrb_module_get_f(state, "FFI");
    struct RClass *module = mrb_module_get_under_f(state, FFI, "CExt");
    struct RClass *klass = mrb_class_get_under_f(state, module, "drram_stats");
    drb_typecheck_aggregate_f(state, self, klass, &ForeignObjectType_ZTS9drram_stats);
    return ((struct drb_foreign_object_ZTS9drram_stats *)DATA_PTR(self))->value;
}
static mrb_value drb_ffi__ZTS9drram_stats_ToRuby(mrb_state *state, drram_stats value) {
    struct drb_foreign_object_ZTS9drram_stats *ptr = calloc(1, sizeof(struct drb_foreign_object_ZTS9drram_stats));
    ptr->value = value;
    ptr->kind = drb_foreign_object_kind_struct;
    struct RClass *FFI = mrb_module_get_f(state, "FFI");
    struct RClass *module = mrb_module_get_under_f(state, FFI, "CExt");
    struct RClass *klass = mrb_class_get_under_f(state, module, "drram_stats");
    struct RData *rdata = mrb_data_object_alloc_f(state, klass, ptr, &ForeignObjectType_ZTS9drram_stats);
    return mrb_obj_value(rdata);
}
static unsigned long long drb_ffi__ZTSy_FromRuby(mrb_state *state, mrb_value self) {
    drb_typecheck_int_f(state, self);
    return mrb_fixnum(self);
}
static mrb_value drb_ffi__ZTSy_ToRuby(mrb_state *state, unsigned long long value) {
    return mrb_fixnum_value(value);
}
static mrb_value drb_ffi__ZTS9drram_stats_New(mrb_state *state, mrb_value self) {
    struct drb_foreign_object_ZTS9drram_stats *ptr = calloc(1, sizeof(struct drb_foreign_object_ZTS9drram_stats *));
    struct RClass *FFI = mrb_module_get_f(state, "FFI");
    struct RClass *module = mrb_module_get_under_f(state, FFI, "CExt");
    struct RClass *klass = mrb_class_get_under_f(state, module, "drram_stats");
    struct RData *rdata = mrb_data_object_alloc_f(state, klass, ptr, &ForeignObjectType_ZTS9drram_stats);
    return mrb_obj_value(rdata);
}
static mrb_value drb_ffi__ZTS9drram_stats_total_Get(mrb_state *state, mrb_value self) {
    drram_stats record = drb_ffi__ZTS9drram_stats_FromRuby(state, self);
    return drb_ffi__ZTSy_ToRuby(state, record.total);
}
static mrb_value drb_ffi__ZTS9drram_stats_total_Set(mrb_state *state, mrb_value self) {
    mrb_value *args = 0;
    mrb_int argc = 0;
    mrb_get_args_f(state, "*", &args, &argc);
    unsigned long long new_value = drb_ffi__ZTSy_FromRuby(state, args[0]);
    (&((struct drb_foreign_object_ZTS9drram_stats *)DATA_PTR(self))->value)->total = new_value;
    return mrb_nil_value();
}
static mrb_value drb_ffi__ZTS9drram_stats_free_Get(mrb_state *state, mrb_value self) {
    drram_stats record = drb_ffi__ZTS9drram_stats_FromRuby(state, self);
    return drb_ffi__ZTSy_ToRuby(state, record.free);
}
static mrb_value drb_ffi__ZTS9drram_stats_free_Set(mrb_state *state, mrb_value self) {
    mrb_value *args = 0;
    mrb_int argc = 0;
    mrb_get_args_f(state, "*", &args, &argc);
    unsigned long long new_value = drb_ffi__ZTSy_FromRuby(state, args[0]);
    (&((struct drb_foreign_object_ZTS9drram_stats *)DATA_PTR(self))->value)->free = new_value;
    return mrb_nil_value();
}
static mrb_value drb_ffi_drram_Binding(mrb_state *state, mrb_value value) {
    drram_stats ret_val = drram();
    return drb_ffi__ZTS9drram_stats_ToRuby(state, ret_val);
}
static int drb_ffi_init_indirect_functions(void *(*lookup)(const char *));
DRB_FFI_EXPORT
void drb_register_c_extensions(void *(*lookup)(const char *), mrb_state *state, struct RClass *FFI) {
    if (drb_ffi_init_indirect_functions(lookup))
        return;
    struct RClass *module = mrb_define_module_under_f(state, FFI, "CExt");
    struct RClass *object_class = state->object_class;
    mrb_define_module_function_f(state, module, "drram", drb_ffi_drram_Binding, MRB_ARGS_REQ(0));
    mrb_define_module_function_f(state, module, "drram", drb_ffi_drram_Binding, MRB_ARGS_REQ(0));
    struct RClass *drram_statsClass = mrb_define_class_under_f(state, module, "drram_stats", object_class);
    mrb_define_class_method_f(state, drram_statsClass, "new", drb_ffi__ZTS9drram_stats_New, MRB_ARGS_REQ(0));
    mrb_define_method_f(state, drram_statsClass, "total", drb_ffi__ZTS9drram_stats_total_Get, MRB_ARGS_REQ(0));
    mrb_define_method_f(state, drram_statsClass, "total=", drb_ffi__ZTS9drram_stats_total_Set, MRB_ARGS_REQ(1));
    mrb_define_method_f(state, drram_statsClass, "free", drb_ffi__ZTS9drram_stats_free_Get, MRB_ARGS_REQ(0));
    mrb_define_method_f(state, drram_statsClass, "free=", drb_ffi__ZTS9drram_stats_free_Set, MRB_ARGS_REQ(1));
}
static int drb_ffi_init_indirect_functions(void *(*lookup)(const char *fnname)) {
  drb_symbol_lookup = lookup;
  if (!(drb_free_foreign_object_f = (void (*)(mrb_state *, void *)) lookup("drb_free_foreign_object"))) return -1;
  if (!(drb_typecheck_aggregate_f = (void (*)(mrb_state *, mrb_value, struct RClass *, mrb_data_type *)) lookup("drb_typecheck_aggregate"))) return -1;
  if (!(drb_typecheck_bool_f = (void (*)(mrb_state *, mrb_value)) lookup("drb_typecheck_bool"))) return -1;
  if (!(drb_typecheck_float_f = (void (*)(mrb_state *, mrb_value)) lookup("drb_typecheck_float"))) return -1;
  if (!(drb_typecheck_int_f = (void (*)(mrb_state *, mrb_value)) lookup("drb_typecheck_int"))) return -1;
  if (!(mrb_class_get_under_f = (struct RClass *(*)(mrb_state *, struct RClass *, const char *)) lookup("mrb_class_get_under"))) return -1;
  if (!(mrb_data_object_alloc_f = (struct RData *(*)(mrb_state *, struct RClass *, void *, const mrb_data_type *)) lookup("mrb_data_object_alloc"))) return -1;
  if (!(mrb_define_class_method_f = (void (*)(mrb_state *, struct RClass *, const char *, mrb_func_t, mrb_aspec)) lookup("mrb_define_class_method"))) return -1;
  if (!(mrb_define_class_under_f = (struct RClass *(*)(mrb_state *, struct RClass *, const char *, struct RClass *)) lookup("mrb_define_class_under"))) return -1;
  if (!(mrb_define_method_f = (void (*)(mrb_state *, struct RClass *, const char *, mrb_func_t, mrb_aspec)) lookup("mrb_define_method"))) return -1;
  if (!(mrb_define_module_function_f = (void (*)(mrb_state *, struct RClass *, const char *, mrb_func_t, mrb_aspec)) lookup("mrb_define_module_function"))) return -1;
  if (!(mrb_define_module_under_f = (struct RClass *(*)(mrb_state *, struct RClass *, const char *)) lookup("mrb_define_module_under"))) return -1;
  if (!(mrb_exc_get_f = (struct RClass *(*)(mrb_state *, const char *)) lookup("mrb_exc_get"))) return -1;
  if (!(mrb_get_args_f = (mrb_int (*)(mrb_state *, mrb_args_format, ...)) lookup("mrb_get_args"))) return -1;
  if (!(mrb_module_get_f = (struct RClass *(*)(mrb_state *, const char *)) lookup("mrb_module_get"))) return -1;
  if (!(mrb_module_get_under_f = (struct RClass *(*)(mrb_state *, struct RClass *, const char *)) lookup("mrb_module_get_under"))) return -1;
  if (!(mrb_raise_f = (void (*)(mrb_state *, struct RClass *, const char *)) lookup("mrb_raise"))) return -1;
  if (!(mrb_str_new_cstr_f = (mrb_value (*)(mrb_state *, const char *)) lookup("mrb_str_new_cstr"))) return -1;
  return 0;
}
