// Credits: https://www.cemetech.net/forum/viewtopic.php?t=3638&start=0
#include <mruby.h>
#include <string.h>
#include <assert.h>
#include <mruby/string.h>
#include <mruby/data.h>
#include <dragonruby.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BATT_STATE "/proc/acpi/battery/BAT0/state"
#define BATT_INFO "/proc/acpi/battery/BAT0/info"
#define BATT_BUFLEN 256

typedef struct drbat_state {
    unsigned is_charging;
    unsigned sys_state;
    unsigned percent;
    unsigned long long life_time;
} drbat_state;

extern drbat_state drbat(void);

extern drbat_state drbat(void) {
    int battStateHandle;
    long int battRate_mA = 0;
    long int battMax_mAh = 0;
    long int battRemain_mAh = 0;
    char buffer[BATT_READ_BUFLEN];
    char tok_unit[8];
    int readoffset;
    short int readstate = 0;
    short int readlen = 0;

    if (-1 == (battStateHandle = open(PATH_BATT_INFO, O_RDONLY))) {
        return (drbat_state) { 0, 0, 0, 0 };
    }
   
    drbat_state state;
  
    while (readstate < 2) {
        readoffset = 0;
        readstate = 0;
        
        while (!readstate) {
            if (0 > (readlen = read(battStateHandle, buffer + readoffset, 1))) {
                return (drbat_state) { 0, 0, 0, 0 };
            }
            if (!readlen) {
                readstate = 2;
                break;
            }
            if ('\n' == *(buffer + readoffset)) {
                readstate++;
                *(buffer + readoffset + 1) = '\0';
            }
            readoffset++;
        }
        
        if (readstate == 2) break;
        if (NULL != strstr(buffer, "last full capacity")) {
            if (0 >= sscanf(buffer + 25, "%ld %s", &battMax_mAh,tok_unit)) {
                return (drbat_state) { 0, 0, 0, 0 };
            }
            break;
        }
    }
    
    close(battStateHandle);

    if (-1 == (battStateHandle = open(PATH_BATT_STATE,O_RDONLY))) {
        return (drbat_state) { 0, 0, 0, 0 };
    }

    readstate = 0;
    while (readstate < 2) {
        readoffset = 0;
        readstate = 0;
        while (!readstate) {
           if (0 > (readlen = read(battStateHandle, buffer + readoffset, 1))) {
                return (drbat_state) { 0, 0, 0, 0 };
            }
            if (0 == readlen) {
                readstate = 2;
                break;
            }
            if ('\n' == *(buffer + readoffset)) {
                readstate++;
                *(buffer + readoffset + 1) = '\0';
            }
            readoffset++;
        }
        if (readstate == 2) break;
        if (NULL != strstr(buffer, "charging state")) {
            if (NULL != strstr(buffer, "discharging")) state.sys_state = 2;
            else if (NULL != strstr(buffer, "charged")) state.sys_state = 1;
            else state.sys_state = 0;
        } else if (NULL != strstr(buffer, "present rate")) {
            if (0 >= sscanf(buffer + 25, "%ld %s", &battRate_mA, tok_unit)) {
                return (drbat_state) { 0, 0, 0, 0 };
            }
        } else if (NULL != strstr(buffer, "remaining capacity")) {
            if (0 >= sscanf(buffer + 25, "%ld %s", &battRemain_mAh,tok_unit)) {
                return (drbat_state) { 0, 0, 0, 0 };
            }
        }
    }
    
    state.is_charging = (state.sys_state == 1) ? 1 : 0;
    
    close(battStateHandle);

    state.percent = (unsigned) 100 * ((float) battRemain_mAh / (float) battMax_mAh);
   
    if (battRate_mA) {
        unsigned time_hour = (int) (battRemain_mAh / battRate_mA);
        state.life_time = (unsigned long long)(time_hour * 60 * 60);
    }
   
    return state;
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
struct drb_foreign_object_ZTS11drbat_state {
    drb_foreign_object_kind kind;
    drbat_state value;
};
static mrb_data_type ForeignObjectType_ZTS11drbat_state = {"drbat_state", drb_free_foreign_object_indirect};
static drbat_state drb_ffi__ZTS11drbat_state_FromRuby(mrb_state *state, mrb_value self) {
    struct RClass *FFI = mrb_module_get_f(state, "FFI");
    struct RClass *module = mrb_module_get_under_f(state, FFI, "CExt");
    struct RClass *klass = mrb_class_get_under_f(state, module, "Drbat_state");
    drb_typecheck_aggregate_f(state, self, klass, &ForeignObjectType_ZTS11drbat_state);
    return ((struct drb_foreign_object_ZTS11drbat_state *)DATA_PTR(self))->value;
}
static mrb_value drb_ffi__ZTS11drbat_state_ToRuby(mrb_state *state, drbat_state value) {
    struct drb_foreign_object_ZTS11drbat_state *ptr = calloc(1, sizeof(struct drb_foreign_object_ZTS11drbat_state));
    ptr->value = value;
    ptr->kind = drb_foreign_object_kind_struct;
    struct RClass *FFI = mrb_module_get_f(state, "FFI");
    struct RClass *module = mrb_module_get_under_f(state, FFI, "CExt");
    struct RClass *klass = mrb_class_get_under_f(state, module, "Drbat_state");
    struct RData *rdata = mrb_data_object_alloc_f(state, klass, ptr, &ForeignObjectType_ZTS11drbat_state);
    return mrb_obj_value(rdata);
}
static unsigned int drb_ffi__ZTSj_FromRuby(mrb_state *state, mrb_value self) {
    drb_typecheck_int_f(state, self);
    return mrb_fixnum(self);
}
static mrb_value drb_ffi__ZTSj_ToRuby(mrb_state *state, unsigned int value) {
    return mrb_fixnum_value(value);
}
static unsigned long long drb_ffi__ZTSy_FromRuby(mrb_state *state, mrb_value self) {
    drb_typecheck_int_f(state, self);
    return mrb_fixnum(self);
}
static mrb_value drb_ffi__ZTSy_ToRuby(mrb_state *state, unsigned long long value) {
    return mrb_fixnum_value(value);
}
static mrb_value drb_ffi__ZTS11drbat_state_New(mrb_state *state, mrb_value self) {
    struct drb_foreign_object_ZTS11drbat_state *ptr = calloc(1, sizeof(struct drb_foreign_object_ZTS11drbat_state *));
    struct RClass *FFI = mrb_module_get_f(state, "FFI");
    struct RClass *module = mrb_module_get_under_f(state, FFI, "CExt");
    struct RClass *klass = mrb_class_get_under_f(state, module, "Drbat_state");
    struct RData *rdata = mrb_data_object_alloc_f(state, klass, ptr, &ForeignObjectType_ZTS11drbat_state);
    return mrb_obj_value(rdata);
}
static mrb_value drb_ffi__ZTS11drbat_state_is_charging_Get(mrb_state *state, mrb_value self) {
    drbat_state record = drb_ffi__ZTS11drbat_state_FromRuby(state, self);
    return drb_ffi__ZTSj_ToRuby(state, record.is_charging);
}
static mrb_value drb_ffi__ZTS11drbat_state_is_charging_Set(mrb_state *state, mrb_value self) {
    mrb_value *args = 0;
    mrb_int argc = 0;
    mrb_get_args_f(state, "*", &args, &argc);
    unsigned int new_value = drb_ffi__ZTSj_FromRuby(state, args[0]);
    (&((struct drb_foreign_object_ZTS11drbat_state *)DATA_PTR(self))->value)->is_charging = new_value;
    return mrb_nil_value();
}
static mrb_value drb_ffi__ZTS11drbat_state_sys_state_Get(mrb_state *state, mrb_value self) {
    drbat_state record = drb_ffi__ZTS11drbat_state_FromRuby(state, self);
    return drb_ffi__ZTSj_ToRuby(state, record.sys_state);
}
static mrb_value drb_ffi__ZTS11drbat_state_sys_state_Set(mrb_state *state, mrb_value self) {
    mrb_value *args = 0;
    mrb_int argc = 0;
    mrb_get_args_f(state, "*", &args, &argc);
    unsigned int new_value = drb_ffi__ZTSj_FromRuby(state, args[0]);
    (&((struct drb_foreign_object_ZTS11drbat_state *)DATA_PTR(self))->value)->sys_state = new_value;
    return mrb_nil_value();
}
static mrb_value drb_ffi__ZTS11drbat_state_percent_Get(mrb_state *state, mrb_value self) {
    drbat_state record = drb_ffi__ZTS11drbat_state_FromRuby(state, self);
    return drb_ffi__ZTSj_ToRuby(state, record.percent);
}
static mrb_value drb_ffi__ZTS11drbat_state_percent_Set(mrb_state *state, mrb_value self) {
    mrb_value *args = 0;
    mrb_int argc = 0;
    mrb_get_args_f(state, "*", &args, &argc);
    unsigned int new_value = drb_ffi__ZTSj_FromRuby(state, args[0]);
    (&((struct drb_foreign_object_ZTS11drbat_state *)DATA_PTR(self))->value)->percent = new_value;
    return mrb_nil_value();
}
static mrb_value drb_ffi__ZTS11drbat_state_life_time_Get(mrb_state *state, mrb_value self) {
    drbat_state record = drb_ffi__ZTS11drbat_state_FromRuby(state, self);
    return drb_ffi__ZTSy_ToRuby(state, record.life_time);
}
static mrb_value drb_ffi__ZTS11drbat_state_life_time_Set(mrb_state *state, mrb_value self) {
    mrb_value *args = 0;
    mrb_int argc = 0;
    mrb_get_args_f(state, "*", &args, &argc);
    unsigned long long new_value = drb_ffi__ZTSy_FromRuby(state, args[0]);
    (&((struct drb_foreign_object_ZTS11drbat_state *)DATA_PTR(self))->value)->life_time = new_value;
    return mrb_nil_value();
}
static mrb_value drb_ffi_drbat_Binding(mrb_state *state, mrb_value value) {
    drbat_state ret_val = drbat();
    return drb_ffi__ZTS11drbat_state_ToRuby(state, ret_val);
}
static int drb_ffi_init_indirect_functions(void *(*lookup)(const char *));
DRB_FFI_EXPORT
void drb_register_c_extensions(void *(*lookup)(const char *), mrb_state *state, struct RClass *FFI) {
    if (drb_ffi_init_indirect_functions(lookup))
        return;
    struct RClass *module = mrb_define_module_under_f(state, FFI, "CExt");
    struct RClass *object_class = state->object_class;
    mrb_define_module_function_f(state, module, "drbat", drb_ffi_drbat_Binding, MRB_ARGS_REQ(0));
    mrb_define_module_function_f(state, module, "drbat", drb_ffi_drbat_Binding, MRB_ARGS_REQ(0));
    struct RClass *Drbat_stateClass = mrb_define_class_under_f(state, module, "Drbat_state", object_class);
    mrb_define_class_method_f(state, Drbat_stateClass, "new", drb_ffi__ZTS11drbat_state_New, MRB_ARGS_REQ(0));
    mrb_define_method_f(state, Drbat_stateClass, "is_charging", drb_ffi__ZTS11drbat_state_is_charging_Get, MRB_ARGS_REQ(0));
    mrb_define_method_f(state, Drbat_stateClass, "is_charging=", drb_ffi__ZTS11drbat_state_is_charging_Set, MRB_ARGS_REQ(1));
    mrb_define_method_f(state, Drbat_stateClass, "sys_state", drb_ffi__ZTS11drbat_state_sys_state_Get, MRB_ARGS_REQ(0));
    mrb_define_method_f(state, Drbat_stateClass, "sys_state=", drb_ffi__ZTS11drbat_state_sys_state_Set, MRB_ARGS_REQ(1));
    mrb_define_method_f(state, Drbat_stateClass, "percent", drb_ffi__ZTS11drbat_state_percent_Get, MRB_ARGS_REQ(0));
    mrb_define_method_f(state, Drbat_stateClass, "percent=", drb_ffi__ZTS11drbat_state_percent_Set, MRB_ARGS_REQ(1));
    mrb_define_method_f(state, Drbat_stateClass, "life_time", drb_ffi__ZTS11drbat_state_life_time_Get, MRB_ARGS_REQ(0));
    mrb_define_method_f(state, Drbat_stateClass, "life_time=", drb_ffi__ZTS11drbat_state_life_time_Set, MRB_ARGS_REQ(1));
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
