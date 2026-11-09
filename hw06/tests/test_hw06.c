#include "sensor.h"
#include "panel.h"
#include "scenario.h"
#include "iom361_r4.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
static int pass = 0, fail = 0;
#define CHECK(name,expr) do { if (expr) {printf("PASS %s\n",name); ++pass;} else {printf("FAIL %s\n",name); ++fail;} } while (0)
static int parse_text(const char *txt, scenario_t *s) {
    char fname[128];
    snprintf(fname,sizeof fname,"/tmp/ece361_hw06_%d.txt",(int)12345);
    FILE *f=fopen(fname,"w"); if (!f) return -1;
    fputs(txt,f); fclose(f);
    int rc=scenario_load(fname,s); remove(fname); return rc;
}
int main(void) {
    int rc = 0;
    uint32_t *base = iom361_initialize(4,4,&rc);
    iom361_set_display(false);
    reading_t r={.t=17,.temp_c=-999,.rh_pct=-999};
    _iom361_set_sensor1(91.0f,55.5f);
    CHECK("sensor 91.0 C",sensor_read(base,100,&r)==0 && r.t==100 && r.temp_c==91.0f && r.rh_pct==55.5f);
    reading_t saved=r;
    CHECK("failed sensor leaves reading unchanged",sensor_read(NULL,101,&r)!=0 && memcmp(&saved,&r,sizeof r)==0);
    CHECK("null reading rejected",sensor_read(base,0,NULL)!=0);
    CHECK("pressed",panel_pressed(0x3,0x1)==0x2);
    CHECK("released",panel_released(0x1,0x3)==0x2);
    CHECK("held switch",panel_pressed(0x5,0x5)==0 && panel_released(0x5,0x5)==0);
    CHECK("led set",panel_led(base,0,1)==0 && panel_led(base,2,1)==0 && iom361_readReg(base,LEDS_REG,&rc)==0x5);
    CHECK("led preserves others",panel_led(base,0,0)==0 && iom361_readReg(base,LEDS_REG,&rc)==0x4);
    CHECK("toggle preserves others",panel_toggle(base,1)==0 && iom361_readReg(base,LEDS_REG,&rc)==0x6);
    CHECK("toggle back",panel_toggle(base,1)==0 && iom361_readReg(base,LEDS_REG,&rc)==0x4);
    CHECK("invalid LED indices",panel_led(base,-1,1)==-1 && panel_toggle(base,32)==-1);
    CHECK("failed led read",panel_led(NULL,1,1)!=0 && panel_toggle(NULL,1)!=0);
    scenario_t *s=malloc(sizeof *s);
    if(!s) return 1;
    CHECK("comments and blanks",parse_text("# header\n\nstart 1791273600 # UTC\n# values\n0 20 30 0x1\n3 21 31 0x2\n",s)==0 && s->count==2);
    CHECK("hold between lines",scenario_at(s,2)==&s->lines[0] && scenario_at(s,3)==&s->lines[1]);
    CHECK("before first line",scenario_at(s,0)==&s->lines[0]);
    CHECK("missing start",parse_text("0 20 30 0x0\n",s)==1);
    CHECK("missing field",parse_text("start 1791273600\n0 20 30\n",s)==2);
    CHECK("non increasing ticks",parse_text("start 1791273600\n0 20 30 0x0\n0 21 31 0x1\n",s)==3);
    CHECK("reject extra field",parse_text("start 1791273600\n0 20 30 0x0 junk\n",s)==2);
    CHECK("nonzero first tick holds model defaults",parse_text("start 1791273600\n2 20 30 0x0\n",s)==0 && scenario_at(s,0)==NULL);
    free(s);
    printf("%d passed, %d failed\n",pass,fail);
    return fail?1:0;
}
