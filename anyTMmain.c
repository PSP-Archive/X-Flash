//					X-Flash Firmware Modification Program
//                     	               For the Sony Playstation Portable (PSP)
//                                   Firmware versions 1.50, 2.71 SE and 3.02 OE
//						    (c) Art 2006
//
//
#include <psppower.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <xflash_files.h>
#include <openinga.h>
#include <openingb.h>
#include <wrench.h>
#include <credits.h>
#include <backgrounds.h>
#include <stars.h>
#include <se5click.h>
#include <se5org.h>
#include <blackboot.h>
#include <about.h>
#include <darkalex.h>
#include <artplate.h>
#include <basp.h>
#include <oasp.h>
#include <sefont.h>
#include <okvag.h>
#include <seblack.h>
#include <psprtc.h>
#include <psphprm.h>
#include <math.h>
#include <pspusb.h>
#include "mikmod.h"
#include <pspaudiolib.h>
#include "ram.h"
#include <ramsig.h>
#include "main.h"
#include "sndch.h"
#include "sndci.h"
#include "sndcr.h"
#include "sndfb.h"
#include "sndgr.h"
#include "sndgv.h"
#include "sndx.h"
#include "sndy.h"
#include "lpcm.h"
#include "pspidstor.h"

#define true 1
#define false 0

PSP_MODULE_INFO("X-FLASH", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

#define printf	pspDebugScreenPrintf

pspTime rtime;

extern int _mm_errno;
extern BOOL _mm_critical;
extern char *_mm_errmsg[];

sample_t* loop_buffer;

u32 state;			// USB mode status variable
u32 freemem;			// free Memory Stick space Mb buffer
float flash_size;		// flash0 sum of file sizes
float mbflash_size;		// flash0 sum of file sizes
int editxmb = 0;		// xmb editor locked status
int nomenutm = 0;		// print menu with message status for topmenu copy
int dect = 0;			// set initial decrypted topmenu_plugin status
int coco;			// found country code status
int xgt;			// delay variable for coco print routine
int topmenu_size;		// topmenu_plugin size for checking routine
int ms_topmenu_size;		// memory stick topmenu_plugin size for checking routine
int calcsig = 0;		// calculate signature on screen status
int sigset;			// set ram signature
int bmperr = 0;			// bitmap error status from X-Flash V18/V18b Auto Restore bug
int fxvf;			//
int fxjan;			//
int fte;			//
int jak;			//
int stingoe = 0;		//
int dcnt;			//
int uhohfs = 0;			// extremely low flash space condition status
int blkk;			//
int mute = 0;			// sample player mute status
int vp;				// delay time
int turbo = 0;			// startup turbo mode
int samt;			//
int samr;			//
int startup = 0;		// startup sequence status for sound loop
int flashsi;			// integer for flash size
int bytesReadf = 1;		// counter for flash size routine
int mikModThreadID = -1;	//
int done = 0;			//
int restore = 0;		// auto restore status
int nodax = 0;			// no custom firmware status
int dodax = 0;			// auto install custom firmware status
int xexp = 0;			// fade circle expand counter
int mcntm;			// demo position memory
int mcrlx = 11;			// credits bar position memory
int mcrla = 16;			//
int mcrlb = 17;			//
int mcrlc = 18;			//
int irshell = 0;		// alternate nickname to allow irshell status indicator
int coinx = 63;			// free space powerup coin coordinate x
int coiny = 12;			// free space powerup coin coordinate y
int coinactive = 0;		// free space powerup coin active status
int fscoin = 0;			// free space power up collect sound control
int barpatch = 0;		// black slider bar patch status
int ooo;			// missing characters for TOP GUN! message
int ggg;			//
int nnn;			//
int lasertime;			// timer for special weapon
int eexplode = 0;		// free space enemy explosion status
int pexplode = 0;		// free space player explosion status
int cexplode = 0;		// free space explosion counter
int stretch;			// control for second special weapon firing line
int amiga;			// Amiga nickname Easter egg trigger status
int prelim;			// preliminary copy status
int checksum;			// checksum buffer for label file crypt routines
int labelsxffail = 0;		// status indicator for file checking routine
int counter;			// counter for label file crypt routines
int dcent = 34;			// demo circle centre
int dircent = 0;		// demo circle centre direction
int stari;			// function variable for starscreen
int fshiscore;			// free space high score buffer
int fsscore = 10800;		// set initial free space score
int fs_up = 0;			// free space up status for moving starscreen
int fs_down = 0;		// free space down status for moving starscreen
int fs_left = 0;		// free space left status for moving starscreen
int fs_right = 0;		// free space right status for moving starscreen
int delay;			// counter
int fsx = 8;			// set initial free space ship position x
int fsy = 17;			// set initial free space ship position y
int fbx = 8;			// set initial free space bullet position x
int fby = 17;			// set initial free space bullet position y
int fsfire = 0;			// set initial free space fire status
int fsalive = 1;		// set initial free space (enemy) alive status
int fsover = 0;			// set free space game over status
int fsshow = 0;			// set initial free space hidden status
int fsehit = 0;			// set initial enemy hit status for sound control
int fsphit = 0;			// set initial player hit status for sound control
int fsehitx = 0;		// set initial enemy destroyed status for sound control
int fshold;			// set message hold timer
int fshits = 0;			// set initial number of hits enemy ship has taken
int freespace;			// Free Space Easter Egg status
int sturm;			// nickname status for LordSturm
int xverx;			// temp version info rotate buffer
int labfileexists = 0;		// for checking existence of Labels.XF file
int labxfsize;			// for checking size of Labels.XF file
int themexfsize;		// for checking size of Theme.XF file
int disselect = 0;		// disable select button status
int disstart = 0;		// disable start button status
int dishome = 0;		// disable home button status
int lcol = 0;			// left column variable to allow slide in of menu
int memsig = 0x3F;		// set checksum seed for RAM signature
int sigcnt;			// memory pseudo array counter
int ram;			//
int ecdir = 0;			//
int eqrad = 1;			// eq radius variable
int wcxc = 0;			// windows effect control
int wc;				//
int xxtx;			//
int xxty;			//
int xxxy;			//
int xxtry;			//
int xo;				// line drawing variables
int yo;				//
int xi;				//
int yi;				//
int bcdir;			//
int art = 0;			// author identification variable
int mmc = 1;			// circle drawing control
int mmb = 1;			//
int mma = 1;			//
int mmd = 10;			//
int twidth;			// temp eq width buffer for rotation of pseudo array
int widtha;			// eq bar width buffers
int widthb;			//
int widthc;			//
int widthd;			//
int widthe;			//
int widthf;			//
int widthg;			//
int widthh;			//
int widthi;			//
int widthj;			//
int widthk;			//
int widthl;			//
int widthm;			//
int widthn;			//
int widtho;			//
int widthp;			//
int widthq;			//
int widthr;			//
int widths;			//
int widtht;			//
int widthu;			//
int widthv;			//
int widthw;			//
int widthx;			//
int n;				//
int fast = 0;			//
int t;				// buffer copy counter
int curspd = 0;			// character rotating speed variable
int colcur;			// cursor colour variable
int cursor = 0;			// editor cursor position
int editcnt = 0;		// editor screen position counteer
int fxt;			// counter for auto restore button delay
int saynos = 0;			// counter for flip finger easter egg
int config_count = 0;		//
int configmenu = 1;		// configurator menu position
int moda = 0;			// no kxploit mod status
int modb = 1;			// hide corrupt icons mod status
int modc = 0;			// skip coldbot intro screen mod status
int modd = 1;			// autoboot user program mod status
int memd;			//
int exitdarkalex = 0;		// don't apply the Dark_AleX mod twice
int exitdarkalexrev = 0;	// don't reverse the Dark_AleX mod twice
int there;			// existence of PSP/SYSTEM/config.txt status
int vshmainsize;		// file size check for vshmain.prx
int sysfolderexists = 0;	//
int dumpfolderexists = 0;	//
int registryfolderexists = 0;	//
int easter = 0;			// easter egg status
int firsttime = 0;		//
int aboutbuff;			// documentation file decrypt buffer
int aboutcnt = 0;		//
int resx = 0;			// background resolution buffers
int resy = 0;			//
int exlowbatt = 0;		// extremely low battery status
int agreement = 0x30;		// agreement status
int fms = 0;			// option 14 sideways status
int qpr = 0;			// option 15 sideways status
int dzm = 0;			// option 1 sideways status
int hilight;			//
int musac;			//
int file_size;			//
int sampls;			//
int stra = 100;			//
int strb = 100;			//
int strc = 100;			//
int strd = 100;			//
int stre = 100;			//
int strf = 100;			//
int strg = 100;			//
int strh = 100;			//
int stri = 100;			//
int strj = 100;			//
int strk = 100;			//
int strl = 100;			//
int strm = 100;			//
int strn = 100;			//
int stro = 100;			//
int starbb;			//
int starda = 10;		//
int stardb = 11;		//
int stardc = 12;		//
int stardd = 13;		//
int starde = 14;		//
int stardf = 15;		//
int stardg = 16;		//
int stardh = 17;		//
int stardi = 18;		//
int stardj = 19;		//
int stardk = 20;		//
int stardl = 21;		//
int stardm = 22;		//
int stardn = 23;		//
int stardo = 24;		//
int cr = 0;			//
int dia = 0xBC;			// diamond character
int alien = 15;			// star character
int greydir = 0;		//
int grey = 0x00666666;		//
int linecount;			//
int smiley = 0x01;		// smiley character 1
int bsmiley = 0x02;		// smiley character 2
int crbuff;			//
int stbuff;			//
int mcnt = 0;			//
int balla = 0;			//
int ballb = 0;			//
int ballx = 30;			//
int bally = 17;			//
int tballa = 0;			//
int tballb = 0;			//
int tballx = 57;		//
int tbally = 21;		//
int crly = 0;			//
int crlx = 11;			//
int crld = 0;			//
int crla = 16;			//
int crlb = 17;			//
int crlc = 18;			//
int tripcol;			//
int test = 0;			//
int ixt;			// delay counter
int fails = 0;			// number of verify fails. hopefully stays at zero
int color = 285887217;		//
int colorb = 285887217;		//
int colorc = 0x00333333;	// non selected menu colour
int colord = 0x00FFFFFF;	//
int colore = 0x00FFFFFF;	//
int disb;			//
int disq;			//
int errs = 1;			//
int colorda;			// dither effect colour buffers
int colordb;			//
int colordc;			//
int colordd;			//
int colorde;			//
int vfa;			// live verify display buffer
int vfb;			//
int vfc;			//
int vfd;			//
int vfe;			//
int vff;			//
int vfg;			//
int vfh;			//
int vfi;			//
int game = 1;			//
int gate;			//
int xxnt;			// wave function variable A
int fxx;			// wave function variable B
int nxx;			// file pointer for update icon function
int chkk;			//
int xxnu;			// counter for update icon function
int menu = 1;			//
int X = 1;			//
int exist = 0;			//
int C = 240;			//
int isplugged = 0;		// Is power cable plugged in? this variable contains the answer.
int batterypower = 0;		// battery power override
int isbatlow = 0;		//
int mlineb = 24;		// set up initial menu line positions
int mlinec = 10;		//
int mlined = 11;		//
int mlinee = 12;		//
int mlinef = 13;		//
int mlineg = 14;		//
int mlinea = 15;		//
int mlineo = 16;		//
int mlineh = 17;		//
int mlinei = 18;		//
int mlinej = 19;		//
int mlinek = 20;		//
int mlinel = 21;		//
int mlinem = 22;		//
int mlinen = 23;		//
int mlinet = 0;			// temporary menu buffer
int fetch;			// line position buffer for dither colour fetch
int esnt;			//
int fes;			//
int vmd;			//
int vmi;			//
int esno;			//
int feo;			//
int qmd;			//
int qmi;			//
int dfdx;			//
int jibber;			//
int gbtrx;			//
int notis = 0;			// not 1.50 firmware status variable
int nottt = 0;			// not 2.71 status variable
int notto = 0;			// not 3.02 status variable
int seb = 0;			// extra Dark_AleX 2.71 SE-B check
int oea = 0;			// Dark_AleX 3.02 OE check
int spreq = 16;			// space required for firmware dump
int sting = 0;			// not available in 2.71 message status flag
int ascii = 0;			// ascii Easter Egg nickname status
int cpa = 8;			// defines the position of characters of interest
int cpb = 9;			//
int cpc = 10;			//
int cpd = 11;			//
int fdo;			//
int month = 1;			//
int verify = 0;			//
int font_file_size;		//
int pmf_file_size;		//
int bmp_file_size;		//
int year;			// 
int started = 0;		// main program has started status for RAM routine
int vshbyte = 0;		// vshmain checking byte for 2.71 SE
int nickaddy = 0;		// nickname address flag
int rawdump = 0;		// raw dump status
int vbmpsize = 6176;		//
int rawdumpexists = 0;		// raw dump folder exists ststus
int openingsize;		// size of opening_plugin.rco file
int topmenusize;		// size of topmenu_plugin.rco file
int systemsize;			// size of system_plugin.rco file
int systembgsize;		// size of system_plugin_bg.rco file
int updatesize;			// size of update_plugin.rco file
int sysconfsize;		// size of sysconf_plugin.rco
int sam = 1;			// sample counter
int samlock = 0;		// player stopped
int samsel = 0;			// sample select
int n;				// sample player variable
char namebuffer[50];		//
char write_buffer[128*1024];	//
unsigned char data[133];	// version checking buffer
unsigned char datab[133];	// index.dat verify buffer
unsigned int buf[5];		// free memory display buffers
unsigned int *pbuf = buf;	//
unsigned char systv[94804];	// system_plugin.rco verify buffer
unsigned char systp[94804];	// system_plugin.rco buffer
unsigned char syscon[313628];	// sysconf_plugin.rco buffer
unsigned char vsys[313628];	// sysconf_plugin.rco verify buffer
unsigned char sysbg[10776];	// system_plugin_bg.rco buffer
unsigned char vbg[10776];	// system_plugin_bg.rco verify buffer
unsigned char update[49404];	// update_plugin.rco buffer
unsigned char vupd[49404];	// update_plugin.rco verify buffer
unsigned char back[25];		// background files buffer
unsigned char ltn0[10];		// ltn0.pgf font checking buffer
unsigned char gbram[200704];	// gameboot.pmf buffer
unsigned char bgbram[331776];	// black gameboot.pmf buffer
unsigned char fntram[2500000];	// ltn0.pgf (and external gameboot) buffer
unsigned char vfntram[2500000];	// ltn0.pgf (and external gameboot) verify buffer
unsigned char opening[266600];	// opening_plugin.rco / topmenu_plugin.rco buffer
unsigned char vopening[266600];	// opening_plugin.rco / topmenu_plugin.rco verify buffer
unsigned char settings[0xB5];	// user settings read from file
unsigned char coldboot[100880];	// coldboot.vag buffer (user supplied sound file)
unsigned char janram[6176];	//
unsigned char vjanram[74112];	//
unsigned char blankpmf[18];	//
unsigned char vblankpmf[18];	//
unsigned char dreg[11360];	// system.dreg buffer for parental control check
unsigned char vshreal[72032];	// real vshmain.prx buffer
unsigned char vvshreal[72032];	// real vshmain.prx verify buffer
unsigned char config[290];	// composed Dark_AleX configuration buffer
unsigned char ebuffer[260];	// menu editor buffer
unsigned char ebuffercomp[260];	// menu editor buffer
unsigned char ebufferxor[260];	// menu editor xor file encrypt table
unsigned char ver[25];		// X-Flash program version info buffer
unsigned char sver[25];		// star tunnel buffer
unsigned char nickname[25];	// nickname buffer
unsigned char allpics[74112];	// consolidated background files buffer
unsigned char sysctl[65454];	// systemctrl.prx buffer (3.02 OE version information)
unsigned char vsysctl[65454];	// systemctrl.prx verify buffer
char Region[512];		// primary Region key buffer
char Serial[512];		// primary Serial key buffer
char Country[512];		// primary Country key buffer
char Mac[512];			// primary MAC address buffer
char USBdes[512];		// primary USB desc buffer
u8 mac0;			// byte size elements for MAC address
u8 mac1;			//
u8 mac2;			//
u8 mac3;			//
u8 mac4;			//
u8 mac5;			//

void write_file(const char *readpath, const char *writepath);
void check(const char* zFile);
void find_sig(void);
void check_nick(void);
void check_music(void);
void boot_music(void);
void warn_flash(void);
void boot_security(void);
void warn_registry(void);
void print_menu(void);
void print_header(void);
void print_footer(void);
void print_blankfooter(void);
void print_credits(void);
void print_creditsb(void);
void crfooter(void);
void clear_midscreen(void);
void scroll_up (void);
void scroll_dn (void);
void read_to_ram(void);
void increment_vers(void);
void decrement_vers(void);
void print_message(void);
void print_regmessage(void);
void print_regrestore(void);
void verify_index(void);
void verify_sysconf(void);
void read_sysconf(void);
void en1337(void);
void de1337(void);
void read_dreg(void);
void read_sysbg(void);
void read_system(void);
void verify_system(void);
void enable_wave(void);
void disable_wave(void);
void verify_sysbg(void);
void read_update(void);
void enable_update(void);
void disable_update(void);
void verify_update(void);
void read_topmenu(void);
void txtdisabled(void);
void txtenabled(void);
void txtgame(void);
void txtcode(void);
void txtexec(void);
void txtgamz(void);
void txt1337(void);
void txtfilz(void);
void txtapps(void);
void txti505(void);
void txt64mz(void);
void txttunes(void);
void txtmusic(void);
void txtaudio(void);
void txtporn(void);
void txtphoto(void);
void txtsnaps(void);
void txtvideo(void);
void txtmovie(void);
void verify_topmenu(void);
void print_boot(void);
void boot_upd(void);
void boot_ltn(void);
void boot_bmp(void);
void boot_bmx(void);
void boot_vag(void);
void boot_power(void);
void boot_prelim(void);
void increment_game(void);
void decrement_game(void);
void increment_dzm(void);
void decrement_dzm(void);
void increment_qpr(void);
void decrement_qpr(void);
void increment_fms(void);
void decrement_fms(void);
void increment_error(void);
void decrement_error(void);
void read_bmp(void);
void read_ltn0(void);
void read_ltn1(void);
void just_boot(void);
void org_error(void);
void mod_error(void);
void org_click(void);
void mod_click(void);
void gameboottoram(void);
void fonttoram(void);
void read_sysctrl(void);
void verify_sysctl(void);
void verify_gameboot(void);
void verify_font(void);
void verify_user_font(void);
void read_font_file(void);
void read_opening(void);
void verify_opening(void);
void enwaveop(void);
void dewaveop(void);
void debrandop(void);
void enbrandop(void);
void debrandgb(void);
void enbrandgb(void);
void delete_opening_vag(void);
void wrench_opening(void);
void original_opening(void);
void actual_delete(void);
void read_settings(void);
void apply_settings(void);
void save_settings(void);
void print_justfooter(void);
void coldboot_opening(void);
void coldboot_echo(void);
void read_coldboot(void);
void bounce_balls(void);
void draw_chicken(void);
void increment_month(void);
void decrement_month(void);
void print_optionf(void);
void alltoram(void);
void jantoram(void);
void febtoram(void);
void martoram(void);
void aprtoram(void);
void maytoram(void);
void juntoram(void);
void jultoram(void);
void augtoram(void);
void septoram(void);
void octtoram(void);
void novtoram(void);
void dectoram(void);
void blktoram(void);
void verify_all(void);
void verify_jan(void);
void verify_feb(void);
void verify_mar(void);
void verify_apr(void);
void verify_may(void);
void verify_jun(void);
void verify_jul(void);
void verify_aug(void);
void verify_sep(void);
void verify_oct(void);
void verify_nov(void);
void verify_dec(void);
void move_arrow(void);
void blank_pmf(void);
void verify_blankpmf(void);
void print_starline(void);
void move_starscreen(void);
void movx_starscreen(void);
void check_gameboot(void);
void boot_bootof(void);
void boot_bootse(void);
void flash_title(void);
void delete_song(void);
void dither(void);
void check_power(void);
void blackgameboottoram(void);
void verify_blackgameboot(void);
void verify_user_gameboot(void);
void read_gameboot_file(void);
void verify_background_forjan(void);
void read_background_forjan(void);
void verify_background_forfeb(void);
void read_background_forfeb(void);
void verify_background_formar(void);
void read_background_formar(void);
void verify_background_forapr(void);
void read_background_forapr(void);
void verify_background_formay(void);
void read_background_formay(void);
void verify_background_forjun(void);
void read_background_forjun(void);
void verify_background_forjul(void);
void read_background_forjul(void);
void verify_background_foraug(void);
void read_background_foraug(void);
void verify_background_forsep(void);
void read_background_forsep(void);
void verify_background_foroct(void);
void read_background_foroct(void);
void verify_background_fornov(void);
void read_background_fornov(void);
void verify_background_fordec(void);
void read_background_fordec(void);
void detmsc(void);
void retmsc(void);
void detmtm(void);
void retmtm(void);
void translator(void);
void read_vshmain(void);
void verify_vshmain(void);
void vshfaketoram(void);
void verify_vshfake(void);
void recoverytoram(void);
void verify_recovery(void);
void configtoram(void);
void verify_config(void);
void vshmaincheck(void);
void vshmaincheckrev(void);
void read_vshmainrev(void);
void verify_vshmainrev(void);
void delete_vshmainreal(void);
void verify_vshmainrealgone(void);
void delete_recovery(void);
void verify_recoverygone(void);
void delete_configuration(void);
void verify_configurationgone(void);
void read_name_plate(void);
void verify_name_plate(void);
void delete_name_plate(void);
void verify_name_plate_gone(void);
void read_png(void);
void boot_png(void);
void print_exit(void);
void another_function(void);
void print_delete_song(void);
void ifgameisstuff(void);
void finger_a(void);
void finger_b(void);
void finger_c(void);
void finger_d(void);
void print_config_menu(void);
void toggle_moda(void);
void toggle_modb(void);
void toggle_modc(void);
void toggle_modd(void);
void increment_configmenu(void);
void decrement_configmenu(void);
void do_configmenu_stuff(void);
void more_configmenu_stuff(void);
void config_footer(void);
void finger_ok_anim(void);
void finger_ok_animB(void);
void finger_stop_anim(void);
void big_intro(void);
void finger_ok_animC(void);
void flip_finger(void);
void finger_patch(void);
void reset_ebuffer(void);
void reset_ebuffercomp(void);
void editor_footer(void);
void print_editor_menu(void);
void invert_cursor(void);
void editor_save(void);
void editor_read(void);
void txtsave(void);
void print_eq(void);
void scroll_freq(void);
void circle_display(void);
void draw_circle(void);
void bounce_circle(void);
void bounce_tcircle(void);
void draw_line(void);
void draw_bar(void);
void draw_line_slow(void);
void draw_windows_circle(void);
void circle_sectora(void);
void circle_sectorb(void);
void bounce_circle_eq(void);
void bounce_circle_eqb(void);
void bounce_circle_eqc(void);
void draw_windows_circleb(void);
void circle_sectorax(void);
void circle_sectorbx(void);
void get_eq_values(void);
void print_verify_passed(void);
void print_verify_failed(void);
void set_program_version(void);
void boot_usb(void);
void boot_mem(void);
void boot_mikmem(void);
void boot_irshell(void);
void boot_notse(void);
void boot_signature(void);
void boot_labels(void);
void boot_theme(void);
void boot_bootoe(void);
void check_front_buttons(void);
void button_disabled(void);
void labels_check(void);
void theme_check(void);
void read_labels(void);
void checkdaseb(void);
void checkdaoea(void);
void draw_demo_circle(void);
void draw_demo_circleb(void);
void draw_demo_circlec(void);
void rotate_ver(void);
void copy_tunnel(void);
void warn_irshell(void);
void five_lines(void);
void save_freespace(void);
void fs_destroyed(void);
void fs_onehitleft(void);
void fs_twohitsleft(void);
void fs_keepitup(void);
void fs_hitreact(void);
void play_freespace(void);
void fs_message(void);
void fs_bullet_collision(void);
void fs_player_destroyed(void);
void set_ebufferxor(void);
void boot_labelsum(void);
void coin_inner(void);
void fs_player_explosion(void);
void fs_enemy_explosion(void);
void print_amiga_weapon(void);
void reset_freespace(void);
void demo_position(void);
void fade_space(void);
void xbasp(void);
void xoasp(void);
void warn_nodax(void);
void warn_fix(void);
void vshmainchecknodax(void);
void check_se(void);
void check_oe(void);
void print_ua_footer(void);
void fix_bmp_error(void);
void write_sysctrl(void);
void orig_sysctrl(void);
void oea_sysctrl(void);
void oec_sysctrl(void);
void en1337_sysctrl(void);
void print_information(void);
void print_coco(void);
void check_topmenu(void);
void ms_check_topmenu(void);
void ms_read_topmenu(void);
void ms_verify_topmenu(void);
void boot_topmenu(void);
void fix_screen(void);
void kws_editor_read(void);
void kws_editor_save(void);
u32 ramAvailableLineareMax(void);
u32 GetRAMFree(void);

void build_path(char *output, const char *root, const char *path, int append) {

	while(*root != 0)
	{
		*output++ = *root++;
	}

	if(*(root-1) != '/')
	{
		*output++ = '/';
	}

	while(*path != 0)
	{
		*output++ = *path++;
	}
	if(append)
		*output++ = '/';

	*output++ = 0;
}

void write_file_dump(const char *read_loc, const char *write_loc, const char *name) {

	int fdin;
	int fdout;
	char readpath[256];
	char writepath[256];

	build_path(readpath, read_loc, name, 0);
	build_path(writepath, write_loc, name, 0);

	fdin = sceIoOpen(readpath, PSP_O_RDONLY, 0777);
	if(fdin >= 0)
	{
		int bytesRead = 1;
		fdout = sceIoOpen(writepath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		if(fdout < 0)
		{
		}

		bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
		while((bytesRead > 0) && (fdout >= 0))
		{
			sceIoWrite(fdout, write_buffer, bytesRead);
			bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
		}

		if(fdout >= 0)
		{
			sceIoClose(fdout);
		}

		if(fdin >= 0)
		{
			sceIoClose(fdin);
		}
	}
}

void dump_flashrom(int lower) {
	int fdin;
	int fdout;

	fdin = sceIoOpen("lflash:", PSP_O_RDONLY, 0777);
	if(fdin > 0)
	{
		int i;
		int bytes_read;

		if(lower)
		{
		fdout = sceIoOpen("./Xflash/Raw_Dump/lower.bin", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else
		{
		fdout = sceIoOpen("./Xflash/Raw_Dump/upper.bin", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		if(fdout > 0)
		{
			for(i = 0; i < ((16 * 1024 * 1024) / sizeof(write_buffer)); i++)
			{
				bytes_read = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
				if(lower)
				{
					sceIoWrite(fdout, write_buffer, bytes_read);
				}
			}

			if(!lower)
			{
				for(i = 0; i < ((16 * 1024 * 1024) / sizeof(write_buffer)); i++)
				{
					bytes_read = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
					sceIoWrite(fdout, write_buffer, bytes_read);
				}
			}

			sceIoClose(fdout);
		}

		sceIoClose(fdin);
	}
	else
	{
		printf("Cannot open lflash: device\n");
		sceKernelSleepThread();
	}
}

void dump_filesystem(const char *root, const char *write_loc) {

	int dfd;
	char next_root[256];
	char next_write[256];

	sceIoMkdir(write_loc, 0777);

	dfd = sceIoDopen(root);
	if(dfd > 0)
	{
		SceIoDirent dir;

		while(sceIoDread(dfd, &dir) > 0)
		{
			if(dir.d_stat.st_attr & FIO_SO_IFDIR)
			{
				if(dir.d_name[0] != '.')
				{
					build_path(next_write, write_loc, dir.d_name, 0);
					build_path(next_root, root, dir.d_name, 1);
					dump_filesystem(next_root, next_write);
				}
			}
			else
			{
				write_file_dump(root, write_loc, dir.d_name);
			}
		}
		sceIoDclose(dfd);
	}
}

void add_file(const char *read_loc, const char *write_loc, const char *name) {

	char readpath[256];

	build_path(readpath, read_loc, name, 0);

	int ffls;
	int filef_size = 0;

	ffls = sceIoOpen(readpath, PSP_O_RDONLY, 0777);
	filef_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	flash_size = flash_size + filef_size;

mbflash_size = flash_size/1024;		// derive Kilobytes used
mbflash_size = mbflash_size/1024;	// derive Megabytes used
flashsi = mbflash_size;			// derive integer for counting display

	pspDebugScreenSetXY(21, 28);
	printf("Flash0 Space Used: %d",flashsi);
	pspDebugScreenSetXY(44, 28);
	printf(" Mb          ");
}

void add_flash(const char *root, const char *write_loc) {

	int dfd;
	char next_root[256];
	char next_write[256];

	dfd = sceIoDopen(root);
	if(dfd > 0)
	{
		SceIoDirent dir;

		while(sceIoDread(dfd, &dir) > 0)
		{
			if(dir.d_stat.st_attr & FIO_SO_IFDIR)
			{
				if(dir.d_name[0] != '.')
				{
					build_path(next_write, write_loc, dir.d_name, 0);
					build_path(next_root, root, dir.d_name, 1);
					add_flash(next_root, next_write);
				}
			}
			else
			{
				add_file(root, write_loc, dir.d_name);
			}
		}
		sceIoDclose(dfd);
	}
}

void audioOutputLoopCallback(void* buf, unsigned int length, void *userdata) {

    sample_t* ubuf = (sample_t*) buf;
	int i;

if (samlock == 1) {
	
if (samsel == 1) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].l = sndch[sam+1] + (sndch[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 38162) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}
	}
}

if (samsel == 11) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].l = lpcm[sam+1] + (lpcm[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 10000) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}
	}
}

if (samsel == 2) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].l = sndci[sam+1] + (sndci[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 9210) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}
	}
}

if (samsel == 3) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].l = sndcr[sam+1] + (sndcr[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 9210) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}

	}
}

if (samsel == 4) {

	n = 0;
	for (i = 0; i < 256; i++) {			// playback 11025 sample
	ubuf[n].l = sndfb[sam+1] + (sndfb[sam]*256);
	ubuf[n].r = ubuf[n].l;
	n++;
	ubuf[n].l = sndfb[sam+1] + (sndfb[sam]*256);
	ubuf[n].r = ubuf[n].l;
	n++;
	ubuf[n].l = sndfb[sam+1] + (sndfb[sam]*256);
	ubuf[n].r = ubuf[n].l;
	n++;
	ubuf[n].l = sndfb[sam+1] + (sndfb[sam]*256);
	ubuf[n].r = ubuf[n].l;
	n++;
	sam++;
	sam++;

		if (sam > 16370) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}


	}
}

if (samsel == 5) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].l = sndgr[sam+1] + (sndgr[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 29509) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}
	}
}

if (samsel == 6) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].l = sndgv[sam+1] + (sndgv[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 29497) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}
	}
}

if (samsel == 7) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].l = sndx[sam+1] + (sndx[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 30310) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}
	}
}

if (samsel == 8) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].l = sndy[sam+1] + (sndy[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 6195) {
		ubuf[i].l = 0;
		ubuf[i].r = 0;
		samlock = 0;
		}
	}
}

if (samsel == 9) {

	n = 0;
	for (i = 0; i < 256; i++) {			// playback 11025 sample
	ubuf[n].r = sndfb[samt+1] + (sndfb[samt]*256);
	n++;
	ubuf[n].r = sndfb[samt+1] + (sndfb[samt]*256);
	n++;
	ubuf[n].r = sndfb[samt+1] + (sndfb[samt]*256);
	n++;
	ubuf[n].r = sndfb[samt+1] + (sndfb[samt]*256);
	n++;
	samt++;
	samt++;

		if (samt > 16370) {
		ubuf[i].r = 0;
		samlock = 0;
		}

	}

	for (i = 0; i < 1024; i++) {
		ubuf[i].l = sndcr[sam+1] + (sndcr[sam]*256);
		sam++;
		sam++;

		if (sam > 9210) {
		ubuf[i].l = 0;
		}
	}

}

if (samsel == 10) {

if (turbo == 1) {

	for (i = 0; i < 1024; i++) {
		ubuf[i].l = lpcm[sam+1] + (lpcm[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;
		sam++;
		sam++;
		sam++;
		sam++;
		sam++;
		sam++;

		if (sam > 160000) {
		ubuf[i].r = 0;
		ubuf[i].l = 0;

	if (startup == 1) {
	if (sam < 160008) {
	sam = 21;
	}
	}
	else {
	samlock = 0;
	samr = 6998;
	}

		}
	}

} // turbo
else {

if (fast == 0) {

	for (i = 0; i < 1024; i++) {
		ubuf[i].l = lpcm[sam+1] + (lpcm[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;

		if (sam > 160000) {
		ubuf[i].r = 0;
		ubuf[i].l = 0;

	if (startup == 1) {
	if (sam < 160004) {
	sam = 21;
	}
	}
	else {
	samlock = 0;
	samr = 6998;
	}

		}
	}

crbuff ++;
if (crbuff > 78) {
SceCtrlData pad;
		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_CROSS) {
		fast = 1;
		}
		else {
		fast = 0;
		}
} // crbuff

} // fast

if (fast > 0) {

	for (i = 0; i < 1024; i++) {
		ubuf[i].l = lpcm[sam+1] + (lpcm[sam]*256);
		ubuf[i].r = ubuf[i].l;
		sam++;
		sam++;
		sam++;
		sam++;

		if (sam > 160000) {
		ubuf[i].r = 0;
		ubuf[i].l = 0;

	if (startup == 1) {
	if (sam < 160004) {
	sam = 21;
	}
	}
	else {
	samlock = 0;
	samr = 6998;
	}

		}
	}

fast ++;
if (fast > 39) {
SceCtrlData vlad;
		sceCtrlReadBufferPositive(&vlad, 1);
		if (vlad.Buttons & PSP_CTRL_CROSS) {
		turbo = 1;
		}
		else {
		turbo = 0;
		}
} // fast

} // fast

} // turbo

}

}

if (samlock == 0) {
int bbb;
	for (bbb = 0; bbb < 1024; bbb++) {
		ubuf[bbb].l = 0xFFFF;
		ubuf[bbb].r = 0xFFFF;
	}
}

if (mute == 1) {
	for (i = 0; i < 1024; i++) {
		ubuf[i].r = 0;
		ubuf[i].l = 0;
	}
}

}

void audioLoopStart() {
	loop_buffer = (sample_t*) malloc(PSP_NUM_AUDIO_SAMPLES * sizeof(sample_t));
	sceKernelDelayThread(200000);	
	pspAudioSetChannelCallback(0, audioOutputLoopCallback, NULL);
}

void my_error_handler(void)
{
pspDebugScreenSetXY(18, 28);
printf("     MIKMOD LIBRARY ERROR ");
return;
}
static int AudioChannelThread(int args, void *argp)
{
  while (!done) {
  MikMod_Update();
  sceKernelDelayThread(1);
  }
  return (0);
}

extern UWORD md_mode;
extern UBYTE md_reverb;
extern UBYTE md_pansep;

int main(void) {

wc = -eqrad;

	MODULE *mf = NULL; // for mod

	int maxchan = 128;
	pspDebugScreenInit();
  
	MikMod_RegisterErrorHandler(my_error_handler);
	MikMod_RegisterAllDrivers();
	MikMod_RegisterAllLoaders();

	md_mode = DMODE_16BITS|DMODE_STEREO|DMODE_SOFT_SNDFX|DMODE_SOFT_MUSIC; 
	md_reverb = 0;
	md_pansep = 128;
  if (MikMod_Init(""))
  {
pspDebugScreenSetXY(18, 28);
printf("     MIKMOD LIBRARY ERROR ");
    pspAudioEnd();
    sceKernelExitGame();
    return 0;
  }
  
  MikMod_SetNumVoices(-1, 8);
  MikMod_EnableOutput();

  if ((mikModThreadID = sceKernelCreateThread("MikMod" ,(void*)&AudioChannelThread,0x12,0x10000,0,NULL)) > 0)
  {
    sceKernelStartThread(mikModThreadID, 0 , NULL);
  }
  else
  {
pspDebugScreenSetXY(18, 28);
printf("     MIKMOD LIBRARY ERROR ");
    pspAudioEnd();
    sceKernelExitGame();
    return 0;
  }

			//*************** X-Flash Program Starts Here ***************

	int i = 0; SceCtrlData pad;

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);

	sceIoUnassign("flash0:"); 
	sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", 0, NULL, 0);

	sceIoUnassign("flash1:"); 
	sceIoAssign("flash1:", "lflash1:0,1", "flashfat1:", 0, NULL, 0);

	read_settings(); //read user settings from file

	loop_buffer = NULL;
	pspAudioInit();
	audioLoopStart();

	if (settings[0x47] == 0x31){ //Check for mute sample player setting
	mute = 1;    		        //set RAM mute status if read as set
	}

	if (settings[0x44] == 0x31){ //Check for agreement status save
	agreement = 0x01;            //set RAM agreement status if read as saved
	}

	if (settings[0x45] == 0x31){ //Check for Free Space Easter Egg status save
	freespace = 0x01;            //set Free Space status if read as saved
	}

	if (settings[0x46] != 0){ 		// Check for previous free space high score
	settings[0x46] = settings[0x46] ^ 0x5f;	// simple xor unencrypt high score byte
	fshiscore = settings[0x46] * 40;	// set free space high score to that read from file
	}					//
	if (settings[0x46] == 0){ 		//
	fshiscore = 8040;			// set free space high score to default testing hi score
	}					//

	while (1) {
	if (agreement != 0x01) {     // skip disclaimer screen if already agreed

	settings[0x64] = 0x34; // set a default colour theme because file isn't written yet

	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x000A0AEE);
	pspDebugScreenSetBackColor(0x00000000);

	// English Disclaimer
	pspDebugScreenSetXY(0, 4);
	printf("                             DISCLAIMER:\n\n");
	pspDebugScreenSetTextColor(0x00EFDDCA);
	printf("           This software modifies flash memory of the PSP.\n");
	printf("           As the end user, you accept any risk associated\n");
	printf("               with the use or misuse of this software.\n");
	printf("       This software is provided without warranty of any kind.\n\n");

	printf("          Any software that performs modification of flash\n");
	printf("          files of the PSP has the potential to render the\n");
	printf("              subject PSP unit permanently inoperative.\n\n");
	printf("\n\n\n");

	// German Disclaimer
	pspDebugScreenSetTextColor(0x000A0AEE);
	printf("                       EINVERSTANDNISERKLARUNG: \n\n");
	pspDebugScreenSetTextColor(0x00EFDDCA);
	printf("            Dieses Programm schreibt Dateien in den Flash\n");
	printf("      Speicher deiner PSP, es besteht dadurch ein hohes Risiko,\n");
	printf("              dass die PSP durch eine falsche Datei o.Ä. \n");
	printf("                        unbrauchbar gemacht wird.  \n\n\n\n");

	pspDebugScreenSetTextColor(0x000A0AEE);
	printf("                  PRESS 'X' TO AGREE or 'O' TO EXIT\n\n");
	printf("          THIS SCREEN WILL BE DISABLED UPON YOUR ACCEPTANCE.");
	}// skipped disclaimer screen

	if (agreement != 0x01) {     // skip disclaimer screen if already agreed
	while (1) {

		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CROSS) {
				sam = 1; // set first sample address counter
				samt = 1;// set second sample address counter
				samlock = 1; // re-enable player routine
				samsel = 9; // select button press sample
		sceDisplayWaitVblankStart();

		pspDebugScreenSetBackColor(0x00FFFFFF); //white
		pspDebugScreenSetTextColor(0x00000000);
	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000000);
		pspDebugScreenInit();

	for(i=0; i<60; i++) {
	sceDisplayWaitVblankStart();
	}

		settings[0x44] = 0x31;
		save_settings();

	for(i=0; i<60; i++) {
	sceDisplayWaitVblankStart();
	}

	firsttime = 1;
	break;
	}

	if (pad.Buttons & PSP_CTRL_CIRCLE) {
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000); //black
	pspDebugScreenSetTextColor(0x00008D8F);
	draw_chicken();

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample


	for(i=0; i<120; i++) {
	sceDisplayWaitVblankStart();
	}
   	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	}// skipped disclaimer screen
	}

	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000000);

	read_settings(); //read user settings from file
	apply_settings(); //apply settings
	set_program_version(); //get version info into buffer
	big_intro();
	print_header();
	print_justfooter();
	for(i=0; i<80; i++) {
	sceDisplayWaitVblankStart();
	}

	check_nick();

startup = 1; // set startup sequence status for sound loop
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 10; // select button press sample

if (fast == 0) { vp = 110; } else { vp = 55; }	// set pause delay for either speed
if (turbo == 1) { vp = 27; }			// set turbo speed pause delay

	for(delay=0; delay<vp; delay++) {	// pause delay so the user can see the nickname
	sceDisplayWaitVblankStart();		//
	}					//
	pspDebugScreenSetXY(0, 17);		// print a row of spaces to cover the nickname
	printf("                                                      ");

if (fast == 0) { vp = 50; } else { vp = 25; }	// set pause delay for either speed
if (turbo == 1) { vp = 12; }			// set turbo speed pause delay

	for(delay=0; delay<vp; delay++) {	// short delay with nothing on the screen
	sceDisplayWaitVblankStart();		//
	}					//

	ooo = 0x4F;				// define missing letters for TOP GUN! message
	nnn = 0x4E;				// shown when default high score is beaten
	ggg = 0x47;				//

	sceRtcGetCurrentClockLocalTime(&rtime);	// get the time from the PSP real time clock
	year = rtime.year;			// get the year into the year variable

	if (year > 2015) {			// check if the year is later than 2015
	pspDebugScreenSetXY(0, 17);
	printf("                      Is it really the year %d?",year);
	pspDebugScreenSetXY(0, 18);
	printf("                        Go easy on this PSP eh!");

if (fast == 0) { vp = 200; } else { vp = 100; }	// set pause delay for either speed
if (turbo == 1) { vp = 50; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(0, 17);		// print two rows of spaces to cover the future message
	printf("                                                      ");
	pspDebugScreenSetXY(0, 18);		//
	printf("                                                      ");

if (fast == 0) { vp = 18; } else { vp = 9; }	// set pause delay for either speed
if (turbo == 1) { vp = 4; }			// set turbo speed pause delay

	for(delay=0; delay<vp; delay++) {	// short delay with nothing on the screen
	sceDisplayWaitVblankStart();		//
	}					//
	} // year

	if (year < 2006) {			// check if the year is earlier than 2006
	pspDebugScreenSetXY(0, 17);		// which would mean that the clock is incorrect
	printf("                   THE PSP TIME CLOCK IS NOT SET      ");

if (fast == 0) { vp = 200; } else { vp = 100; }	// set pause delay for either speed
if (turbo == 1) { vp = 50; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(0, 17);		// print a row of spaces to cover the message
	printf("                                                      ");

if (fast == 0) { vp = 18; } else { vp = 9; }	// set pause delay for either speed
if (turbo == 1) { vp = 4; }			// set turbo speed pause delay

	for(delay=0; delay<vp; delay++) {	// short delay with nothing on the screen
	sceDisplayWaitVblankStart();		//
	}					//
	} // year

	check_front_buttons();			// check for stuck down front buttons

	set_ebufferxor();			// set values for random table array

	while (1) {

		isplugged = scePowerIsPowerOnline();
		if (isplugged == 1) {
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(0, 17);
	printf("                        POWER CABLE DETECTED          ");

if (fast == 0) { vp = 85; } else { vp = 42; }	// set pause delay for either speed
if (turbo == 1) { vp = 21; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
		break;
		}
		if (batterypower == 1) {
		sceDisplayWaitVblankStart();
		break;
		}
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(0, 17);
	printf("                     POWER CABLE WAS NOT DETECTED     ");

		if(scePowerIsLowBattery()) {
		sceDisplayWaitVblankStart();
		isbatlow = 1;
		}
		if (isbatlow == 0) {
	pspDebugScreenSetXY(0, 19);
	printf("                          BATTERY LEVEL IS OK         ");

if (fast == 0) { vp = 85; } else { vp =42; }	// set pause delay for either speed
if (turbo == 1) { vp = 21; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
		break;
		}
		else {
	pspDebugScreenSetXY(0, 19);
	printf("                         BATTERY LEVEL IS LOW         ");
	pspDebugScreenSetXY(0, 21);
	printf("                    INSERT POWER CABLE TO CONTINUE    ");
		}

		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_RIGHT) {
			        for(i=0; i<999; i++) {
		isplugged = scePowerIsPowerOnline();
		if (isplugged == 1) {
		sceDisplayWaitVblankStart();
		break;
		}
		sceDisplayWaitVblankStart();
		}

		for(i=0; i<4; i++) {
		isplugged = scePowerIsPowerOnline();
		if (isplugged == 1) {
		sceDisplayWaitVblankStart();
		break;
		}
		sceDisplayWaitVblankStart();
		}

		while (1) {
		isplugged = scePowerIsPowerOnline();
		if (isplugged == 1) {
		sceDisplayWaitVblankStart();
		break;
		}
		sceCtrlReadBufferPositive(&pad, 1);

		}

if (fast == 0) { vp = 90; } else { vp = 45; }	// set pause delay for either speed
if (turbo == 1) { vp = 22; }			// set turbo speed pause delay

		for(i=0; i<vp; i++) {
		sceDisplayWaitVblankStart();
		}
		}
	}

	pspDebugScreenSetXY(0, 17);
	printf("                                                      ");
	pspDebugScreenSetXY(0, 19);
	printf("                                                      ");
	pspDebugScreenSetXY(0, 21);
	printf("                                                      ");

if (fast == 0) { vp = 18; } else { vp = 9; }	// set pause delay for either speed
if (turbo == 1) { vp = 4; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetXY(6, 6);
	read_to_ram();

	pspDebugScreenSetXY(0, 17);
	printf("                    PSP UNIT FIRMWARE VERSION ");
	printf("%c",data[cpa]);
	printf("%c",data[cpb]);
	printf("%c",data[cpc]);
	printf("%c",data[cpd]);
	printf("\n");

	notis = 0;

	if (data[cpa] != 49) {
	notis = 1;
	}
	if (data[cpc] != 53) {
	notis = 1;
	}
	if (data[cpd] != 48) {
	notis = 1;
	}

	if (notis == 1) {	// not 1.50 firmware, so check for 2.71
	checkdaseb();
	if (nottt == 1) {	// not 2.71 firmware either, so check for 3.02
	checkdaoea();
	if (notto == 1) {	// not 3.02 firmware either, so give the boot
	print_boot();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}
	}

	if (notis == 1) {
	check_se();
	if (seb == 0) {		// not Dark_AleX 2.71 SE
	check_oe();
	if (seb != 2) {		// not Dark_AleX 3.02 OE
	boot_notse();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}

if (fast == 0) { vp = 10; } else { vp = 5; }	// set pause delay for either speed
if (turbo == 1) { vp = 2; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(0, 17);

if (seb == 1) {
	printf("                 PSP UNIT FIRMWARE VERSION 2.71 SE ");
}
if (seb == 2) {
	printf("                 PSP UNIT FIRMWARE VERSION 3.02 OE ");
seb = 1; //
oea = 1; //
}
	}

if (fast == 0) { vp = 85; } else { vp = 42; }	// set pause delay for either speed
if (turbo == 1) { vp = 21; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	
	if (seb == 0) {
		openingsize = 254344;
		topmenusize = 230856;
		systemsize = 94804;
		sysconfsize = 313628;
		systembgsize = 10776;
		updatesize = 49404;
	}
	if (seb == 1) {
		openingsize = 231720;
		topmenusize = 193104;
		systemsize = 83892;
		sysconfsize = 253856;
		systembgsize = 8352;
		updatesize = 13904;
	}
	if (oea == 1) {
		openingsize = 231756;
		topmenusize = 210836;
		systemsize = 84016;
		sysconfsize = 289096;
		systembgsize = 8352;
		updatesize = 13904;
	}

	pspDebugScreenSetXY(0, 17);
	printf("                                                    ");

check_topmenu(); // get the size of topmenu_plugin.rco on flash0

if (topmenu_size == 247988) {	// check actual size of topmenu_plugin.rco
if (oea == 1) {			//
topmenusize = 247988;		// set new topmenu_plugin size for program routines
dect = 1;			// set decrypted topmenu_plugin status
}
}

//if (dect == 0) {
//if (oea == 1) {
//if (topmenu_size != 210836) {	// check size of topmenu_plugin.rco
//boot_topmenu();
//	pspAudioEnd();
//	sceKernelExitGame();
//	return 0;
//}
//}
//}

//if (oea == 0) {
//if (seb == 1) {
//if (topmenu_size != 193104) {	// check size of topmenu_plugin.rco
//boot_topmenu();
//	pspAudioEnd();
//	sceKernelExitGame();
//	return 0;
//}
//}
//}

if (oea == 0) {
if (seb == 0) {
if (topmenu_size != 230856) {	// check size of topmenu_plugin.rco
boot_topmenu();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
}
}
}

if (fast == 0) { vp = 30; } else { vp = 15; }	// set pause delay for either speed
if (turbo == 1) { vp = 7; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	if (seb == 0) {
	if (nickaddy == 1) {
	pspDebugScreenSetXY(0, 17);
	printf("                    INCORRECT REGISTRY VERSION     ");

if (fast == 0) { vp = 30; } else { vp = 15; }	// set pause delay for either speed
if (turbo == 1) { vp = 7; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	warn_registry();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	}
	}

	if (seb == 1) {
	if (nickaddy == 0) {
	pspDebugScreenSetXY(0, 17);
	printf("                    INCORRECT REGISTRY VERSION     ");

if (fast == 0) { vp = 30; } else { vp = 15; }	// set pause delay for either speed
if (turbo == 1) { vp = 7; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	warn_registry();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	}
	}

	if (oea == 1) {
	if (nickaddy == 0) {
	pspDebugScreenSetXY(0, 17);
	printf("                    INCORRECT REGISTRY VERSION     ");

if (fast == 0) { vp = 30; } else { vp = 15; }	// set pause delay for either speed
if (turbo == 1) { vp = 7; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	warn_registry();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	}
	}

	if (oea == 1) {
	if (nickaddy == 1) {
	pspDebugScreenSetXY(0, 17);
	printf("                    INCORRECT REGISTRY VERSION     ");

if (fast == 0) { vp = 30; } else { vp = 15; }	// set pause delay for either speed
if (turbo == 1) { vp = 7; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	warn_registry();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	}
	}

	if (seb == 0) {
	if (nickaddy == 2) {
	pspDebugScreenSetXY(0, 17);
	printf("                    INCORRECT REGISTRY VERSION     ");

if (fast == 0) { vp = 30; } else { vp = 15; }	// set pause delay for either speed
if (turbo == 1) { vp = 7; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	warn_registry();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	}
	}

fix_bmp_error(); // detect and remove incorrect version files left by X-Flash V18/V18b
if (bmperr == 1) {
warn_fix();
pspDebugScreenSetBackColor(0x00000000);
pspDebugScreenInit();
print_header();
print_justfooter();
pspDebugScreenSetTextColor(0x00EFDDCA);
bmperr = 0;
}

	read_dreg(); // check parental control setting
	pspDebugScreenSetXY(0, 17);
	printf("                    PSP PARENTAL CONTROL LEVEL ");

if (seb == 0) { //
	if (dreg[4188] != 0) {
	if (dreg[4188] != 1) {
	pspDebugScreenSetXY(0, 17);
	printf("                  PSP PARENTAL CONTROL LEVEL ERROR ");

if (fast == 0) { vp = 83; } else { vp = 41; }	// set pause delay for either speed
if (turbo == 1) { vp = 20; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_security();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}
} // seb
if (seb == 1) { //
if (oea == 0) { //
	if (dreg[9820] != 0) {
	if (dreg[9820] != 1) {
	pspDebugScreenSetXY(0, 17);
	printf("                  PSP PARENTAL CONTROL LEVEL ERROR ");

if (fast == 0) { vp = 83; } else { vp = 41; }	// set pause delay for either speed
if (turbo == 1) { vp = 20; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_security();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}
} // oea
} // seb

if (seb == 1) { //
if (oea == 1) { //
	if (dreg[11356] != 0) {
	if (dreg[11356] != 1) {
	pspDebugScreenSetXY(0, 17);
	printf("                  PSP PARENTAL CONTROL LEVEL ERROR ");

if (fast == 0) { vp = 83; } else { vp = 41; }	// set pause delay for either speed
if (turbo == 1) { vp = 20; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_security();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}
} // oea
} // seb

if (seb == 0) { //
	if (dreg[4188] == 0) {
	printf("OFF");
	}
	if (dreg[4188] == 1) {
	printf("1");
	}
} // seb
if (seb == 1) { //
if (oea == 0) { //
	if (dreg[9820] == 0) {
	printf("OFF");
	}
	if (dreg[9820] == 1) {
	printf("1");
	}
} // oea
} // seb
if (seb == 1) { //
if (oea == 1) { //
	if (dreg[11356] == 0) {
	printf("OFF");
	}
	if (dreg[11356] == 1) {
	printf("1");
	}
} // oea
} // seb

	data[cpa] = 57; // set menu default version tricker value 9.99
	data[cpc] = 57;
	data[cpd] = 57;

if (fast == 0) { vp = 85; } else { vp = 42; }	// set pause delay for either speed
if (turbo == 1) { vp = 21; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(0, 17);
	printf("                                                   ");

	print_header();
	print_justfooter();

if (fast == 0) { vp = 6; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000000);

	pspDebugScreenSetXY(21, 28); // print 6AM sample player version to debug window
	printf("    6AM Sample Player     ");
	pspDebugScreenSetXY(21, 29);
	printf("       version 1b         ");

if (fast == 0) { vp = 90; } else { vp = 45; }	// set pause delay for either speed
if (turbo == 1) { vp = 22; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(21, 28); // print blank lines
	printf("                          ");
	pspDebugScreenSetXY(21, 29);
	printf("                          ");

if (fast == 0) { vp = 10; } else { vp = 5; }	// set pause delay for either speed
if (turbo == 1) { vp = 2; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(21, 28); // print Mikmod sound library revision to debug window
	printf("   MikMod Sound Library   ");
	pspDebugScreenSetXY(21, 29);
	printf("      version %ld.%ld.%ld",
	LIBMIKMOD_VERSION_MAJOR,
	LIBMIKMOD_VERSION_MINOR,
	LIBMIKMOD_REVISION);

if (fast == 0) { vp = 90; } else { vp = 45; }	// set pause delay for either speed
if (turbo == 1) { vp = 22; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(21, 28);
	printf("                             ");
	pspDebugScreenSetXY(21, 29);
	printf("                             ");

//	if (prelim == 0) {		// comment out for distribution versions,
//	boot_prelim();			// or include it to require "Prelim" as PSP nickname
//	pspAudioEnd();
//	sceKernelExitGame();		//
//	return 0;			//
//	}				//

if (fast == 0) { vp = 8; } else { vp = 4; }	// set pause delay for either speed
if (turbo == 1) { vp = 2; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(21, 28);
	printf("CHECKING FW: update_plugin.rco ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	read_update(); //check for previously wrecked update_plugin.rco file
	gate = 1;
	if (update[1008] == 0xFF) {
	gate = 0;
	}
	if (gate == 0) {
	boot_upd();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(21, 28);
	printf("CHECKING MS: ltn0.pgf           ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	read_ltn0(); //check for invalid font
	if (disq == 1) {
	boot_ltn();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(21, 28);
	printf("CHECKING MS: Coldboot.vag       ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	read_coldboot();
	if (coldboot[0] != 0x56) {  //Check for 'V' of "VAG file"
	boot_vag();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (coldboot[0x21] != 0x6F) {  //Check for 'o' of "coldboot" in file header // commented out for V14a
	boot_vag();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (coldboot[0x22] != 0x6C) {  //Check for 'l' of "coldboot" in file header
	boot_vag();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (coldboot[0x27] != 0x74) {  //Check for 't' of "coldboot" in file header
	boot_vag();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(21, 28);
	printf("CHECKING MS: sndcx.XF          ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	check_music();

	if (file_size != 1159791) {  //  <= Check music file size
	boot_music();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	if (gbram[0x23] != 0x58) {  //  "X"  <= Check for missing or altered music file
	boot_music();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (gbram[0x22] != 0x41) {  //  "A"
	boot_music();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (gbram[0x11] != 0x46) {  //  "F" low Motion
	boot_music();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (gbram[0x21] != 0x52) {  //  "R"
	boot_music();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (gbram[0x20] != 0x44) {  //  "D"
	boot_music();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(21, 28);
	printf("CHECKING MS: Theme.XF ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	theme_check();
	if (themexfsize != 181) {
	for(i=0; i<25; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_theme();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(21, 28);
	printf("CHECKING MS: Labels.XF ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	labels_check();
	if (labfileexists == 1) {
	if (labxfsize != 261) {
	for(i=0; i<24; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_labels();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}

	if (labelsxffail == 1) {
	for(i=0; i<24; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_labelsum();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(21, 28);
	printf("                                 ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	read_bmp(); //check bmp files
	if (disb == 1) {
	boot_bmp();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(18, 28);
	printf("BACKGROUND RESOLUTION: %d x %d",resx,resy);

if (fast == 0) { vp = 93; } else { vp = 46; }	// set pause delay for either speed
if (turbo == 1) { vp = 23; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(18, 28);
	printf("                             ");

	if (resx > 60) {  // Check if background.bmp is bigger than 6176 bytes 60x34.
	pspDebugScreenSetXY(21, 28);
	printf("WARNING: HI-RES BACKGROUND!      ");

if (fast == 0) { vp = 93; } else { vp = 46; }	// set pause delay for either speed
if (turbo == 1) { vp = 23; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	}

	if (resy > 34) {  // Check if background.bmp is bigger than 6176 bytes 60x34.
	pspDebugScreenSetXY(21, 28);
	printf("WARNING: HI-RES BACKGROUND!      ");

if (fast == 0) { vp = 93; } else { vp = 46; }	// set pause delay for either speed
if (turbo == 1) { vp = 23; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	}

	if (resx > 480) {  // Check if background.bmp is bigger than 6176 bytes 60x34.
	pspDebugScreenSetXY(21, 28);
	printf("WARNING: OVER SIZE BACKGROUND!   ");

if (fast == 0) { vp = 93; } else { vp = 46; }	// set pause delay for either speed
if (turbo == 1) { vp = 23; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_bmp();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	if (resy > 272) {  // Check if background.bmp is bigger than 6176 bytes 60x34.
	pspDebugScreenSetXY(21, 28);
	printf("WARNING: OVER SIZE BACKGROUND!   ");

if (fast == 0) { vp = 93; } else { vp = 46; }	// set pause delay for either speed
if (turbo == 1) { vp = 23; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_bmp();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

if (oea == 1) {
	if (resx != 60) {  // Check if background.bmp is 60x34.
	pspDebugScreenSetXY(21, 28);
	printf("WARNING: WRONG SIZE BACKGROUND!  ");

if (fast == 0) { vp = 93; } else { vp = 46; }	// set pause delay for either speed
if (turbo == 1) { vp = 23; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_bmx();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	if (resy != 34) {  // Check if background.bmp is 60x34.
	pspDebugScreenSetXY(21, 28);
	printf("WARNING: WRONG SIZE BACKGROUND!  ");

if (fast == 0) { vp = 93; } else { vp = 46; }	// set pause delay for either speed
if (turbo == 1) { vp = 23; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	boot_bmx();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
} // oea

	pspDebugScreenSetXY(21, 28);
	printf("                                 ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	read_png(); //check bmp files
	if (disb == 1) {
	boot_png();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(18, 28);
	printf("NAME PLATE RESOLUTION: %d x %d",resx,resy);

if (fast == 0) { vp = 93; } else { vp = 46; }	// set pause delay for either speed
if (turbo == 1) { vp = 23; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(18, 28);
	printf("                              ");


	if (resx > 480) {  // Check if name_plate.png is bigger than 480x272.
	boot_png();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	if (resy > 272) {  // Check if name_plate.png is bigger than 480x272.
	boot_png();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(21, 28);
	printf("                                 ");

if (fast == 0) { vp = 7; } else { vp = 3; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	check_gameboot();

	pspDebugScreenSetXY(21, 28);
	printf("GAMEBOOT SIZE: %d BYTES",file_size);

if (fast == 0) { vp = 33; } else { vp = 16; }	// set pause delay for either speed
if (turbo == 1) { vp = 8; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(21, 29);
	if (seb == 0) {
	printf("      MAXIMUM: 2500000 BYTES");	// print gameboot size limitation
	}
	if (seb == 1) {
	if (oea == 0) {
	printf("      MAXIMUM: 632832 BYTES ");
	}
	}
	if (seb == 1) {
	if (oea == 1) {
	printf("      MAXIMUM: 200704 BYTES ");
	}
	}

if (fast == 0) { vp = 39; } else { vp = 19; }	// set pause delay for either speed
if (turbo == 1) { vp = 9; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	if (seb == 0) {
	if (file_size > 2500000) {  //  <= Check gameboot.pmf file size for 1.50
	boot_bootof();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}

	if (seb == 1) {
	if (oea == 0) {
	if (file_size > 632832) {  //  <= Check gameboot.pmf file size for 2.71 SE
	boot_bootse();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}
	}

	if (seb == 1) {
	if (oea == 1) {
	if (file_size > 200704) {  //  <= Check gameboot.pmf file size for 2.71 SE
	boot_bootoe();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}
	}

	if (file_size != 0) {		// <= Check valid gameboot.pmf if not a zero byte file
	if (gbram[0x0] != 0x50) {  //  "P"
	if (seb == 0) {
	boot_bootof();
	}
	else {
	boot_bootse();
	}
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (gbram[0x1] != 0x53) {  //  "S"
	if (seb == 0) {
	boot_bootof();
	}
	else {
	boot_bootse();
	}
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	if (gbram[0x2] != 0x4D) {  //  "M"
	if (seb == 0) {
	boot_bootof();
	}
	else {
	boot_bootse();
	}
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	}

if (fast == 0) { vp = 5; } else { vp = 2; }	// set pause delay for either speed
if (turbo == 1) { vp = 1; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(16, 29);
	printf("                                  ");

if (fast == 0) { vp = 26; } else { vp = 13; }	// set pause delay for either speed
if (turbo == 1) { vp = 6; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	if (sturm == 0) {
	if (sceKernelFindModuleByName("IRSHELL")) {	// comment this out for using
	boot_irshell();
	pspAudioEnd();					// iR Shell to take snapshots
	sceKernelExitGame();				// or use the nickname Easter egg
	return 0;					//
	}						//
	}

	pspDebugScreenSetXY(18, 28);
	printf("                                      ");

if (fast == 0) { vp = 16; } else { vp = 8; }	// set pause delay for either speed
if (turbo == 1) { vp = 4; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(21, 28);
	printf("Flash0 Space Used: ");

if (fast == 0) { vp = 18; } else { vp = 9; }	// set pause delay for either speed
if (turbo == 1) { vp = 4; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	flash_size = 0;
	add_flash("flash0:/", "./Xflash/Dump/flash0");

	mbflash_size = flash_size/1024;		// derive Kilobytes used
	mbflash_size = mbflash_size/1024;	// derive Megabytes used

	pspDebugScreenSetXY(21, 28);
	printf("Flash0 Space Used: %f Mb",mbflash_size);
	pspDebugScreenSetXY(44, 28);
	printf(" Mb          ");

if (fast == 0) { vp = 18; } else { vp = 9; }	// set pause delay for either speed
if (turbo == 1) { vp = 4; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	mbflash_size = 20.2 - mbflash_size;	// derive approximate free space

	if (mbflash_size < 0) { 		// make sure it isn't less than zero
	uhohfs = 1;				// extremely low space condition
	mbflash_size = 0;
	}

//	pspDebugScreenSetXY(21, 29);
//	printf("   Estimated Free: %f Mb",mbflash_size);
//	pspDebugScreenSetXY(43, 29);
//	printf(" Mb          ");

if (fast == 0) { vp = 80; } else { vp = 40; }	// set pause delay for either speed
if (turbo == 1) { vp = 20; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(18, 28);
	printf("                                      ");
	pspDebugScreenSetXY(18, 29);
	printf("                                      ");

if (fast == 0) { vp = 8; } else { vp = 4; }	// set pause delay for either speed
if (turbo == 1) { vp = 2; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	if (uhohfs == 1) {  // Check flash space left
	warn_flash();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	}

if (fast == 0) { vp = 8; } else { vp = 4; }	// set pause delay for either speed
if (turbo == 1) { vp = 2; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	pspDebugScreenSetXY(18, 28);
	printf("                                      ");
	pspDebugScreenSetXY(18, 29);
	printf("                                      ");

	GetRAMFree();					// check for RAM avbailable
	started = 1;					// set main program started status

	pspDebugScreenSetXY(18, 28);
	printf("                                      ");
	pspDebugScreenSetXY(18, 29);
	printf("                                      ");

if (fast == 0) { vp = 8; } else { vp = 4; }	// set pause delay for either speed

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	find_sig();

	pspDebugScreenSetXY(25, 28);

	for(sigcnt=0; sigcnt<14; sigcnt++) {
	printf("%c",ramsig[sigcnt]);
	}

	for(sigcnt=20; sigcnt<1200100; sigcnt++) {
	pspDebugScreenSetXY(39, 28);
	memsig = (memsig ^ ramsig[sigcnt]);
	if (calcsig == 1) {
	printf(":%02x",memsig);	// show calc for distribution versions
	} // calcsig
	}

	if (calcsig == 1) {
	for(i=0; i<400; i++) {
	sceDisplayWaitVblankStart();
	}
	} // calcsig

	if (memsig != sigset) { //
	boot_signature();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	pspDebugScreenSetXY(39, 28);
	printf("OK  ");

if (fast == 0) { vp = 45; } else { vp = 22; }	// set pause delay for either speed
if (turbo == 1) { vp = 11; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	state = sceUsbGetState();
	if (state & PSP_USB_ACTIVATED) {
	boot_usb();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	startup = 0; // reset startup sequence status for sound loop

	pspDebugScreenSetXY(16, 28);
	printf("                                    ");

if (fast == 0) { vp = 18; } else { vp = 9; }	// set pause delay for either speed
if (turbo == 1) { vp = 4; }			// set turbo speed pause delay

	for(i=0; i<vp; i++) {
	sceDisplayWaitVblankStart();
	}

	if (sturm == 1) {
	if (sceKernelFindModuleByName("IRSHELL")) {
	warn_irshell();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	}
	}

		sceCtrlReadBufferPositive(&pad, 1);	// handbrake for holding X when menu starts
		if (pad.Buttons & PSP_CTRL_CROSS) {
	pspDebugScreenSetXY(16, 28);
	printf(" TAKE YOUR FINGER OFF THE X BUTTON ! ");
	pspDebugScreenSetXY(16, 29);
	printf("  THERE'S NO NEED TO HOLD IT DOWN !  ");
	for(i=0; i<400; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(18, 28);
	printf("                                     ");
	pspDebugScreenSetXY(18, 29);
	printf("                                     ");
	for(i=0; i<18; i++) {
	sceDisplayWaitVblankStart();
	}
	}

	if (firsttime == 1) {				//
	vshmainchecknodax();

	if (nodax == 1) {
	warn_nodax();

	while (1) {
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CROSS) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
		sceDisplayWaitVblankStart();
		pspDebugScreenSetBackColor(0x00FFFFFF); //white
		pspDebugScreenSetTextColor(0x00000000);
		pspDebugScreenInit();
		pspDebugScreenSetTextColor(0x00EFDDCA);
		pspDebugScreenSetBackColor(0x00000000);
		pspDebugScreenInit();
		for(i=0; i<60; i++) {
		sceDisplayWaitVblankStart();
		}
		dodax = 1; // set auto install custom firmware status
		break;
		}

		if (pad.Buttons & PSP_CTRL_CIRCLE) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
		sceDisplayWaitVblankStart();
		pspDebugScreenSetBackColor(0x00FFFFFF); //white
		pspDebugScreenSetTextColor(0x00000000);
		pspDebugScreenInit();
		pspDebugScreenSetTextColor(0x00EFDDCA);
		pspDebugScreenSetBackColor(0x00000000);
		pspDebugScreenInit();
		for(i=0; i<60; i++) {
		sceDisplayWaitVblankStart();
		}
		break; // auto install custom firmware is already clear
		}
		}
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	}
	}

	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000000);

	for(config_count=0; config_count<290; config_count++) { // compose config.txt file from original
	config[config_count] = configmaster[config_count];
	}

	copy_tunnel(); // copy tunnel stars from star file

	print_menu();
	finger_patch();

	state = sceUsbGetState();
	if (state & PSP_USB_ACTIVATED) {
	boot_usb();
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	while (1) {

if (firsttime == 1) {
menu = 15;
qpr = 3;
print_blankfooter();
			for(i=0; i<38; i++) {
		        sceDisplayWaitVblankStart();
			}

	pspDebugScreenSetTextColor(colorb);
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';  X-Flash has opened the EULA for first use.  ;`-';");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';    Please take a moment to read the EULA.    ;`-';");
goto eula;
}

	if (fails > 2) {	// exit program for too many verify fails
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  MULTIPLE VERIFY FAILURES ");
	for(i=0; i<200; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(18, 28);
	printf("         PROGRAM EXIT      ");
	for(i=0; i<110; i++) {
	sceDisplayWaitVblankStart();
	}
	pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

	sceCtrlSetSamplingMode(1); 
	sceCtrlReadBufferPositive(&pad, 1);

	if (test != 9) {

if(pad.Lx > 235) {
		if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		increment_vers();
		print_menu();
goto fast;
		}

	}

if(pad.Lx < 20) {
		if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		decrement_vers();
		print_menu();
goto fast;
		}

	}

   if(pad.Ly < 20) {
			grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample
			if (menu > 0) {
				menu--;
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
          		}
			if (menu == 0) {
			menu = 15;
			}
		scroll_dn();
		}

   if(pad.Ly > 235) {
			grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample
			if (menu < 16) {
				menu++;
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}
			if (menu == 16) {
			menu = 1;
			}
		scroll_up();
		}

   if(pad.Lx < 30) {
			grey = 0x00555555;
			if (menu == 13) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		                sceDisplayWaitVblankStart();

				settings[0x64]--;
				if (settings[0x64] == 0x30) {
				settings[0x64] = 0x3F;
				}

				apply_settings();
				print_blankfooter();

				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(18, 28);
				hilight = colord;
				flash_title();
			}

			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				decrement_vers();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 7) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				decrement_game();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 2) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				decrement_error();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			decrement_dzm();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}

			if (menu == 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			decrement_month();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}

			if (menu == 15) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			decrement_qpr();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}

			if (menu == 14) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			decrement_fms();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}


		}

   if(pad.Lx > 225) {
			grey = 0x00555555;
			if (menu == 13) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		                sceDisplayWaitVblankStart();

				settings[0x64]++;
				if (settings[0x64] == 0x40) {
				settings[0x64] = 0x31;
				}

				apply_settings();
				print_blankfooter();

				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(18, 28);
				hilight = colord;
				flash_title();
			}

			if (menu == 7) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_game();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_vers();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 2) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_error();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_dzm();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_month();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 15) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			increment_qpr();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}

			if (menu == 14) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			increment_fms();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}

		}
		}
fast:
		sceCtrlReadBufferPositive(&pad, 1);

	if (test != 9) {

		if (pad.Buttons & PSP_CTRL_UP) {
			grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample

			if (menu > 0) {
				menu--;
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
          		}
			if (menu == 0) {
			menu = 15;
			}
		scroll_dn();
		}


		if (pad.Buttons & PSP_CTRL_DOWN) {
			grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample

			if (menu < 16) {
				menu++;
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}
			if (menu == 16) {
			menu = 1;
			}
		scroll_up();
		}

		if(pad.Buttons & PSP_CTRL_RTRIGGER) {
		grey = 0x00555555;
			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		increment_vers();
		print_menu();
		}

		}

		if(pad.Buttons & PSP_CTRL_LTRIGGER) {
		grey = 0x00555555;
			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		decrement_vers();
		print_menu();
		}
		}

		if (pad.Buttons & PSP_CTRL_LEFT) {
			grey = 0x00555555;
			if (menu == 13) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		                sceDisplayWaitVblankStart();

				settings[0x64]--;
				if (settings[0x64] == 0x30) {
				settings[0x64] = 0x3F;
				}

				apply_settings();
				print_blankfooter();

				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(18, 28);
				hilight = colord;
				flash_title();
			}

			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				decrement_vers();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 7) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				decrement_game();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}


			if (menu == 2) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				decrement_error();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				decrement_dzm();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				decrement_month();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 15) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			decrement_qpr();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}

			if (menu == 14) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			decrement_fms();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}
		}

		if (pad.Buttons & PSP_CTRL_RIGHT) {
			grey = 0x00555555;
			if (menu == 13) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		                sceDisplayWaitVblankStart();

				settings[0x64]++;
				if (settings[0x64] == 0x40) {
				settings[0x64] = 0x31;
				}

				apply_settings();
				print_blankfooter();

				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(18, 28);
				hilight = colord;
				flash_title();
			}

			if (menu == 7) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_game();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_vers();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 2) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_error();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_dzm();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
				increment_month();
				print_menu();
			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

			if (menu == 15) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			increment_qpr();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}

			if (menu == 14) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
			increment_fms();
			print_menu();
			for(i=0; i<9; i++) {
		        sceDisplayWaitVblankStart();
			}
			}
		}
		}

	if (menu != X) {
		print_menu();
		X = menu;
	}








		if (pad.Buttons & PSP_CTRL_TRIANGLE) {
			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample

if (oea == 1) {	//*****
oec_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
			printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****

if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		}

			if (menu == 10) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample

if (oea == 1) {	//*****
oec_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
			printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****

if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		}

		}

		if (pad.Buttons & PSP_CTRL_SQUARE) {
		if (menu == 8) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
		blackgameboottoram();
		verify_blackgameboot();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		}

			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample

if (oea == 1) {	//*****
oea_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
			printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****

if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		}

			if (menu == 10) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample

if (oea == 1) {	//*****
oea_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
			printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****

if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		}

		}

		if (pad.Buttons & PSP_CTRL_CROSS) {
		grey = 0x00555555;

			if (menu == 15) {
if (qpr == 5) {					// play stand alone free space game
reset_freespace();				// reset free space game
mcnt = 200;					//
fshold = 0;					// remove unlock message
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
		clear_midscreen();
		hilight = colord;
		flash_title();
		hilight = color;
		flash_title();

	crfooter();
	pspDebugScreenSetXY(25, 17);
		pspDebugScreenSetTextColor(colord);
	printf(" LOADING MUSIC...");
GetRAMFree();

				mf = Player_Load("./xflash/Xflash/sndcx.XF", maxchan, 0);
        if (NULL != mf)
        {
          mf->wrap = 1;
				Player_Start(mf);
        }
	pspDebugScreenSetXY(0, 17);
	pspDebugScreenSetTextColor(colorc);
	printf("                                      ");
	print_credits();

		while (1) {
				int qi;
				print_credits();
			        for(qi=0; qi<3; qi++) {
		    	   	sceDisplayWaitVblankStart();
				}
if (fsover == 0) {
if (fsfire == 1) {
if (fbx == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
if (fs_left == 1) {
if (fbx - 4 == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
}
}
}

if (fsehit == 1) {
fsehit = 0;
fsfire = 0; // 
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}
if (fsehitx == 1) {
fsehitx = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}
if (fsphit == 1) {
fsphit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}

if (fscoin == 1) {
fscoin = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}

		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_CIRCLE) {
       		if (NULL != mf) {
		Player_Stop();
		Player_Free(mf);
       		mf = NULL;
		}
		grey = 0x00555555;
				if (test == 1) {
				//voice = Sample_Play(ch,0,0);
				//Voice_SetVolume(voice, vol);
				//Voice_SetPanning(voice, pan);
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample
				}
				else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
				}
				reset_freespace();
		                sceDisplayWaitVblankStart();
				break;
				}

				print_creditsb();
			        for(qi=0; qi<3; qi++) {
		    	   	sceDisplayWaitVblankStart();
				}

if (fsover == 0) {
if (fsfire == 1) {
if (fbx == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
if (fs_left == 1) {
if (fbx - 4 == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
}
}
}

if (fsehit == 1) {
fsfire = 0; // 
fsehit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}
if (fsehitx == 1) {
fsehitx = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}
if (fsphit == 1) {
fsphit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}

if (fscoin == 1) {
fscoin = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}

		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_CIRCLE) {
       		if (NULL != mf) {
		Player_Stop();
		Player_Free(mf);
       		mf = NULL;
		}
		grey = 0x00555555;
				if (test == 1) {
				//voice = Sample_Play(ch,0,0);
				//Voice_SetVolume(voice, vol);
				//Voice_SetPanning(voice, pan);
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample
				}
				else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
				}
				reset_freespace();
		                sceDisplayWaitVblankStart();
				break;
				}

if (fsshow == 0) {
crfooter();			// only refresh credits footer outside of free space mode
}

				print_creditsb();
			        for(qi=0; qi<3; qi++) {
		    	   	sceDisplayWaitVblankStart();
				}
if (fsover == 0) {
if (fsfire == 1) {
if (fbx == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
if (fs_left == 1) {
if (fbx - 4 == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
}
}
}

if (fsehit == 1) {
fsfire = 0; // 
fsehit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}
if (fsehitx == 1) {
fsehitx = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}
if (fsphit == 1) {
fsphit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}

if (fscoin == 1) {
fscoin = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}

		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_CIRCLE) {
       		if (NULL != mf) {
		Player_Stop();
		Player_Free(mf);
       		mf = NULL;
		}
		grey = 0x00555555;
				if (test == 1) {
				//voice = Sample_Play(ch,0,0);
				//Voice_SetVolume(voice, vol);
				//Voice_SetPanning(voice, pan);
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample
				}
				else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
				}
				reset_freespace();
		                sceDisplayWaitVblankStart();
				break;
				}

if (mcnt == 470) {
sceDisplayWaitVblankStart();
mcnt = 200;
crfooter();
fade_space();
reset_freespace();
clear_midscreen();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(0, 17);
	printf("                            FREE SPACE        ");
for(i=0; i<120; i++) {
sceDisplayWaitVblankStart();
}
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(0, 17);
	printf("                                              ");
for(i=0; i<50; i++) {
sceDisplayWaitVblankStart();
}
}
		}
		clear_midscreen();
			        for(i=0; i<40; i++) {
		                sceDisplayWaitVblankStart();
				}
		print_menu();
}

if (qpr == 4) {
		mcnt = mcntm;	// set demo position
crlx = mcrlx;			// credits bar position memory
crla = mcrla;			//
crlb = mcrlb;			//
crlc = mcrlc;			//
	fsshow = 0;		// hide game
	fsover = 0;		// free space game can be restarted
	fsshow = 0;		// hide game
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
		clear_midscreen();
		hilight = colord;
		flash_title();
		hilight = color;
		flash_title();
		read_sysconf(); // check for 13.37 version spoof applied
		crfooter();
	pspDebugScreenSetXY(25, 17);
	pspDebugScreenSetTextColor(colord);
	printf(" LOADING MUSIC...");
GetRAMFree();

				mf = Player_Load("./xflash/Xflash/sndcx.XF", maxchan, 0);
        if (NULL != mf)
        {
          mf->wrap = 1;
				Player_Start(mf);
        }
	pspDebugScreenSetXY(0, 17);
		pspDebugScreenSetTextColor(colorc);
	printf("                                      ");
	print_credits();

		while (1) {
				int qi;
				print_credits();
			        for(qi=0; qi<3; qi++) {
		    	   	sceDisplayWaitVblankStart();
				}
if (fsover == 0) {
if (fsfire == 1) {
if (fbx == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
if (fs_left == 1) {
if (fbx - 4 == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
}
}
}

if (fsehit == 1) {
fsfire = 0; // 
fsehit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}
if (fsehitx == 1) {
fsehitx = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}
if (fsphit == 1) {
fsphit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}

if (fscoin == 1) {
fscoin = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}

		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_CIRCLE) {
       		if (NULL != mf) {
		Player_Stop();
		Player_Free(mf);
       		mf = NULL;
		}
		demo_position();
		grey = 0x00555555;
				if (test == 1) {
				//voice = Sample_Play(ch,0,0);
				//Voice_SetVolume(voice, vol);
				//Voice_SetPanning(voice, pan);
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample
				}
				else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
				}
		                sceDisplayWaitVblankStart();
				break;
				}

if (fsshow == 0) {
if (pad.Buttons & PSP_CTRL_TRIANGLE) {
if (menu == 15) {

if (chkk == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

easter++;
if (easter == 2) {
easter = 0;
}
}

if (oea == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

easter++;
if (easter == 2) {
easter = 0;
}
}

}
}
}


				print_creditsb();
			        for(qi=0; qi<3; qi++) {
		    	   	sceDisplayWaitVblankStart();
				}

if (fsover == 0) {
if (fsfire == 1) {
if (fbx == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
if (fs_left == 1) {
if (fbx - 4 == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
}
}
}

if (fsehit == 1) {
fsehit = 0;
fsfire = 0; // 
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}
if (fsehitx == 1) {
fsehitx = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}
if (fsphit == 1) {
fsphit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}

if (fscoin == 1) {
fscoin = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}

		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_CIRCLE) {
       		if (NULL != mf) {
		Player_Stop();
		Player_Free(mf);
       		mf = NULL;
		}
		demo_position();
		grey = 0x00555555;
				if (test == 1) {
				//voice = Sample_Play(ch,0,0);
				//Voice_SetVolume(voice, vol);
				//Voice_SetPanning(voice, pan);
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample
				}
				else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
				}
		                sceDisplayWaitVblankStart();
				break;
				}

if (fsshow == 0) {
if (pad.Buttons & PSP_CTRL_TRIANGLE) {
if (menu == 15) {

if (chkk == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

easter++;
if (easter == 2) {
easter = 0;
}
}

if (oea == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

easter++;
if (easter == 2) {
easter = 0;
}
}

}
}
crfooter();			// only refresh credits footer outside of free space mode
}


				print_creditsb();
			        for(qi=0; qi<3; qi++) {
		    	   	sceDisplayWaitVblankStart();
				}
if (fsover == 0) {
if (fsfire == 1) {
if (fbx == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
if (fs_left == 1) {
if (fbx - 4 == fsx) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
}
}
}

if (fsehit == 1) {
fsfire = 0; // 
fsehit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}
if (fsehitx == 1) {
fsehitx = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}
if (fsphit == 1) {
fsphit = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
}

if (fscoin == 1) {
fscoin = 0;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
}

if (mcnt > 471) { // turn on credits bar after star display
fsover = 0;		// free space game can be restarted
fsalive = 1;		// revive enemy
fshits = 0;		// reset number of hits taken
fsfire = 0;		// kill current bullet
fsscore = 10800;	// reset score
fsshow = 0;		// hide game
amiga = 0;		// turn off super weapon
fsx = 8;		// reset ship position
fsy = 17;		//
fbx = 8;		// reset bullet position
fby = 17;		//
}

		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_CIRCLE) {
       		if (NULL != mf) {
		Player_Stop();
		Player_Free(mf);
       		mf = NULL;
		}
		demo_position();
		grey = 0x00555555;
				if (test == 1) {
				//voice = Sample_Play(ch,0,0);
				//Voice_SetVolume(voice, vol);
				//Voice_SetPanning(voice, pan);
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample
				}
				else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
				}
		                sceDisplayWaitVblankStart();
				break;
				}

if (fsshow == 0) {
if (pad.Buttons & PSP_CTRL_TRIANGLE) {
if (menu == 15) {
if (chkk == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
easter++;
if (easter == 2) {
easter = 0;
}
}

if (oea == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
easter++;
if (easter == 2) {
easter = 0;
}
}

}
}
}

		}

		clear_midscreen();
			        for(i=0; i<40; i++) {
		                sceDisplayWaitVblankStart();
				}
		print_menu();
		}

eula:
		if (qpr == 3) {
		grey = 0x00555555;
if (firsttime == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
}
		clear_midscreen();
if (firsttime == 0) {
		hilight = colord;
		flash_title();
		hilight = color;
		flash_title();
}
		aboutcnt = 1298;
		read_sysconf(); // check for 13.37 version spoof applied
		while (1) {
				int xqi;
				int masve;

	for(masve=10; masve<25; masve++) {
	for(xqi=6; xqi<62; xqi++) {
	pspDebugScreenSetXY(xqi, masve);
	pspDebugScreenSetTextColor(colord);
	aboutbuff = (about[aboutcnt] ^ 0x3F);	//decrypt simple xor encrypted document

							if (easter == 1) {  // trigger easter egg 1337 translator
if (aboutbuff == 0x41) {
aboutbuff = 0x34;
}
if (aboutbuff == 0x61) {
aboutbuff = 0x34;
}
if (aboutbuff == 0x42) {
aboutbuff = 0x38;
}
if (aboutbuff == 0x62) {
aboutbuff = 0x38;
}
if (aboutbuff == 0x43) {
aboutbuff = 0x28;
}
if (aboutbuff == 0x63) {
aboutbuff = 0x28;
}
if (aboutbuff == 0x45) {
aboutbuff = 0x33;
}
if (aboutbuff == 0x65) {
aboutbuff = 0x33;
}
if (aboutbuff == 0x47) {
aboutbuff = 0x36;
}
if (aboutbuff == 0x67) {
aboutbuff = 0x36;
}
if (aboutbuff == 0x4C) {
aboutbuff = 0x31;
}
if (aboutbuff == 0x6C) {
aboutbuff = 0x31;
}
if (aboutbuff == 0x4F) {
aboutbuff = 0x30;
}
if (aboutbuff == 0x6F) {
aboutbuff = 0x30;
}
if (aboutbuff == 0x53) {
aboutbuff = 0x35;
}
if (aboutbuff == 0x73) {
aboutbuff = 0x35;
}
if (aboutbuff == 0x54) {
aboutbuff = 0x37;
}
if (aboutbuff == 0x74) {
aboutbuff = 0x37;
}
} // end of easter egg code

	printf("%c",aboutbuff);
	aboutcnt++;
	}
	aboutcnt++;
	aboutcnt++;
	aboutcnt++;
	}

if (aboutcnt > 27610 ) {
aboutcnt = 1298;
}
if (aboutcnt < 1298 ) {
aboutcnt = 27611;
}
	pspDebugScreenSetXY(0, 25);
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.; Use D pad to scroll text reader up and down. ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';    Press Circle to return to X-Flash menu.   ;`-';");
	pspDebugScreenSetXY(0, 29);
if (easter == 1) {  // alert to easter egg triggered
	printf("      ;.-.;          1337 7r4n51a70r 4(7|va73d!          ;.-.;");
}
else {
	printf("      ;.-.;                                              ;.-.;");
}

	pspDebugScreenSetXY(0, 31);
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");

		while (1) {
		sceDisplayWaitVblankStart();
		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_DOWN) {
		aboutcnt = aboutcnt - 826;
		break;
		}
		if (pad.Buttons & PSP_CTRL_UP) {
		aboutcnt = aboutcnt - 944;
		break;
		}

   if(pad.Ly > 235) {
		aboutcnt = aboutcnt - 826;
		break;
		}

   if(pad.Ly < 20) {
		aboutcnt = aboutcnt - 944;
		break;
		}

if (pad.Buttons & PSP_CTRL_TRIANGLE) {
if (menu == 15) {
if (chkk == 0) {

aboutcnt = aboutcnt - 885;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
easter++;
if (easter == 2) {
easter = 0;
}
break;
}


if (oea == 1) {

aboutcnt = aboutcnt - 885;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

			        for(i=0; i<9; i++) {
		                sceDisplayWaitVblankStart();
				}
easter++;
if (easter == 2) {
easter = 0;
}
break;
}


}
}

		if (pad.Buttons & PSP_CTRL_CIRCLE) {
		break;
		}
		}

		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_CIRCLE) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
		clear_midscreen();
		sceDisplayWaitVblankStart();
		break;
		}

		}
		clear_midscreen();

if (firsttime == 1) {
if (dodax == 0) {
menu = 15;
firsttime = 0;
scroll_dn();
}

if (dodax == 1) {
dodax = 0;
menu = 14;
firsttime = 0;
scroll_dn();
scroll_dn();
fms = 2;
}
}
			        for(i=0; i<40; i++) {
		                sceDisplayWaitVblankStart();
				}
		print_menu();
		}

if (qpr == 2) {
	if (test == 0) {
	//voice = Sample_Play(ch,0,0);
	//Voice_SetVolume(voice, vol);
	//Voice_SetPanning(voice, pan);

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample

	dumpfolderexists = 0;
	color = 0x00008D8F; //test mode theme
	colorb = 0x00008D8F;
	colorc = 0x00333333;
	colord = 0x00008D8F;
	print_header();
	print_justfooter();
	draw_chicken();
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("       CHECK DUMP FOLDER      ");
	for(jibber=0; jibber<60; jibber++) {
	sceDisplayWaitVblankStart();
	}

	dfdx = sceIoDopen("./Xflash/Dump/flash0");
	if(dfdx >= 0) {
	dumpfolderexists = 1;
	}

if (dumpfolderexists == 0) {


	apply_settings();
	print_header();
	print_menu();
	print_justfooter();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
	finger_stop_anim();
	print_blankfooter();
	pspDebugScreenSetXY(18, 28);
	printf("  PERFORM FIRMWARE DUMP FIRST ");

	for(jibber=0; jibber<120; jibber++) {
	sceDisplayWaitVblankStart();
	}
}
else {
another_function();
}

	}
	else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	test = 0;	//reset test mode status
	apply_settings();
	print_header();
	print_justfooter();
			        for(i=0; i<11; i++) {
		                sceDisplayWaitVblankStart();
				}
	}
}

if (qpr == 0) {
fxt = 0;
		print_blankfooter();
		finger_patch();
while (1)	   {
		   sceCtrlReadBufferPositive(&pad, 1);
		   if (pad.Buttons & PSP_CTRL_CROSS) {
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
			clear_midscreen();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 17);
	printf("         PROGRAM EXIT      ");
fxt = 0;
while (1)	   {
		SceCtrlData sleep;				// 'pad' buffer for standby hold button detect
		sceCtrlReadBufferPositive(&sleep, 1);		// detect extra long button hold
		if (sleep.Buttons & PSP_CTRL_CROSS) {
if (barpatch == 1) {
		pspDebugScreenSetTextColor(0x00000000);		// black (cover up buggy bar)
}
else {

		if (settings[0x64] == 0x3E) {
		pspDebugScreenSetTextColor(2109782);		// metallic bronze colour
		}
		else {
		pspDebugScreenSetTextColor(0x001111FE);		// red for standby slider bar
		}

}
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
		print_blankfooter();
			clear_midscreen();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 17);
	printf("           PSP OFF         ");
fxt = 0;
barpatch = 1;	// set standby status for black bar patch
			int scePowerRequestStandby(void);
			scePowerRequestStandby();
			}
}
			print_blankfooter();
    pspAudioEnd();
			sceKernelExitGame();
			return 0;
			}
}
}

if (qpr == 1) {
	fxt = 0;
	print_blankfooter();
while (1)	   {
		   int ix;
		   sceCtrlReadBufferPositive(&pad, 1);
		   if (pad.Buttons & PSP_CTRL_CROSS) {
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
		print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(19, 28);
	printf("        AUTO RESTORE       ");
		      for(ix=0; ix<90; ix++) {
		      sceDisplayWaitVblankStart();
		      }

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

			read_opening(); //restore branded opening screen
			enbrandop();
			verify_opening();
			read_opening(); //restore branded gameboot screen
			enbrandgb();
			verify_opening();
if (oea == 0) {
			read_system();	//restore original error sound
			org_error();
			verify_system();
			read_system();	//restore original click sound
			org_click();
			verify_system();
} // oea
			read_opening();
			delete_opening_vag();//restore original opening sound
			original_opening();
			verify_opening();
			read_update();  //restore network update icon function
			enable_update();
			verify_update();
			read_sysbg();	//enable XMB wave effect
			enable_wave();
			verify_sysbg();
			read_opening(); //enable opening screen wave effect
			enwaveop();
			verify_opening();

if (oea == 0) {
jantoram();				//restore original backgrounds
verify_jan();
febtoram();
verify_feb();
martoram();
verify_mar();
aprtoram();
verify_apr();
maytoram();
verify_may();
juntoram();
verify_jun();
jultoram();
verify_jul();
augtoram();
verify_aug();
septoram();
verify_sep();
octtoram();
verify_oct();
novtoram();
verify_nov();
dectoram();
verify_dec();
} // oea
if (oea == 1) {
alltoram();
verify_all();
}
delete_name_plate();
verify_name_plate_gone();

		fonttoram();	//restore original font
		verify_font();
		gameboottoram(); //restore original gameboot movie
		verify_gameboot();

if (seb == 0) {

read_topmenu(); // restore menu labels
reset_ebuffer();
if (dect == 0) {
		editor_save();
}
else {
		kws_editor_save();
}
txtsave();
verify_topmenu();
			read_topmenu(); //restore tm symbols
			retmtm();
			verify_topmenu();
} // seb

if (oea == 1) {	//*****
orig_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
			printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****
if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}

			restore = 1; // switch on auto restore status

if (seb == 0) {
			read_sysconf();
			retmsc();
			xoasp();
			verify_sysconf();
			print_justfooter();
			pspDebugScreenSetXY(0, 28);
			pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
			printf("      ;`-';                ");
			pspDebugScreenSetTextColor(colord);
			printf("RESTORE index.dat");
			pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
			printf("             ;`-';");

		read_sysconf(); //remove version 13.37 hack
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
		de1337();
		verify_sysconf();

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,data,133); // restore original 1.50 index.dat file
		sceIoClose(fdo);
		verify_index();
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
			print_justfooter();
			pspDebugScreenSetXY(0, 28);
			pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
			printf("      ;`-';                ");
			pspDebugScreenSetTextColor(colord);
			printf(" DELETE SONG  ");
			pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
			printf("                ;`-';");
			delete_song();
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
			vshmaincheckrev();
			if (exitdarkalexrev == 0) {
			read_vshmainrev();
			verify_vshmainrev();
			delete_vshmainreal();
			verify_vshmainrealgone();
			delete_recovery();
			verify_recoverygone();
			delete_configuration();
			verify_configurationgone();
			}
	}// seb
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			clear_midscreen();
			print_justfooter();
			print_exit();
			pspAudioEnd();
			sceKernelExitGame();
			return 0;
			}
			}
			}
			}

			if (menu == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

			if (dzm == 0) {
			read_opening();
			debrandop();
			verify_opening();
			}
			if (dzm == 1) {
			read_opening();
			debrandgb();
			verify_opening();
			}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();

			}
			if (menu == 2) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

			if (errs == 3) {
			read_opening();
			delete_opening_vag();
			coldboot_echo();
			verify_opening();
			}
			if (errs == 4) {
			read_opening();
			delete_opening_vag();
			wrench_opening();
			verify_opening();
			}
			if (errs == 5) {
			read_opening();
			delete_opening_vag();
			coldboot_opening();
			verify_opening();
			}
			if (errs == 6) {
			read_opening();
			delete_opening_vag();
			actual_delete();
			verify_opening();
			}

if (oea == 0) {
			if (errs == 1) {
			read_system();
			mod_error();
			verify_system();
			}
			if (errs == 2) {
			read_system();
			org_error();
			verify_system();
			}
			if (errs == 7) {
			read_system();
			mod_click();
			verify_system();
			}
			if (errs == 8) {
			read_system();
			org_click();
			verify_system();
			}
}
else {
if (errs == 1) {
sting = 1;
stingoe = 1;
}
if (errs == 2) {
sting = 1;
stingoe = 1;
}
if (errs == 7) {
sting = 1;
stingoe = 1;
}
if (errs == 8) {
sting = 1;
stingoe = 1;
}
}
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}

			if (menu == 3) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
			read_update();
			disable_update();
			verify_update();
	if (seb == 0) {
			read_topmenu();
			txtdisabled();
			verify_topmenu();
	}
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}

			if (menu == 4) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
			read_sysbg();
			disable_wave();
			verify_sysbg();
			read_opening();
			dewaveop();
			verify_opening();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}

			if (menu == 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

if (month == 1) {
read_background_forjan();
verify_background_forjan();
}
if (month == 2) {
read_background_forfeb();
verify_background_forfeb();
}
if (month == 3) {
read_background_formar();
verify_background_formar();
}
if (month == 4) {
read_background_forapr();
verify_background_forapr();
}
if (month == 5) {
read_background_formay();
verify_background_formay();
}
if (month == 6) {
read_background_forjun();
verify_background_forjun();
}
if (month == 7) {
read_background_forjul();
verify_background_forjul();
}
if (month == 8) {
read_background_foraug();
verify_background_foraug();
}
if (month == 9) {
read_background_forsep();
verify_background_forsep();
}
if (month == 10) {
read_background_foroct();
verify_background_foroct();
}
if (month == 11) {
read_background_fornov();
verify_background_fornov();
}
if (month == 12) {
read_background_fordec();
verify_background_fordec();
}
if (month == 13) {
read_background_forjan();
verify_background_forjan();
read_background_forfeb();
verify_background_forfeb();
read_background_formar();
verify_background_formar();
read_background_forapr();
verify_background_forapr();
read_background_formay();
verify_background_formay();
read_background_forjun();
verify_background_forjun();
read_background_forjul();
verify_background_forjul();
read_background_foraug();
verify_background_foraug();
read_background_forsep();
verify_background_forsep();
read_background_foroct();
verify_background_foroct();
read_background_fornov();
verify_background_fornov();
read_background_fordec();
verify_background_fordec();
}

if (month == 14) {
blktoram();
}

if (month == 15) {

if (oea == 0) {
jantoram();
verify_jan();
febtoram();
verify_feb();
martoram();
verify_mar();
aprtoram();
verify_apr();
maytoram();
verify_may();
juntoram();
verify_jun();
jultoram();
verify_jul();
augtoram();
verify_aug();
septoram();
verify_sep();
octtoram();
verify_oct();
novtoram();
verify_nov();
dectoram();
verify_dec();
} // oea
if (oea == 1) {
alltoram();
verify_all();
}

}

if (month == 16) {
read_name_plate();
verify_name_plate();
}

if (month == 17) {

	if (seb == 0) {

read_sysconf();
xbasp();
verify_sysconf();
} // seb
else {
sting = 1;
}
}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	print_message();
			}
			if (menu == 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
			read_font_file();
			verify_user_font();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}
			if (menu == 7) {
if (game != 19) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	if (seb == 0) {
			read_topmenu();
			if (game == 18) {
			detmtm();
			verify_topmenu();
			read_sysconf();
			detmsc();
			verify_sysconf();
			}

			else {
			ifgameisstuff();
			verify_topmenu();
			}
	} // seb
	else {
	sting = 1; // set unavailable in 2.71 SE message status
	}
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}

if (game == 19) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
editxmb = 0;

if (seb == 0) { // allow menu editor with 1.50 firmware
editxmb = 1;
}

if (dect == 1) { // allow menu editor with decrypted 2.50 topmenu_plugin.rco
editxmb = 1;
}

if (editxmb == 1) {
	clear_midscreen();
	read_topmenu();
	reset_ebuffer();
	reset_ebuffercomp();
	print_editor_menu();
	editor_footer();
	print_editor_menu();

	for(i=0; i<10; i++) {
	sceDisplayWaitVblankStart();
	}

while (1) {
	int fxncnt;
	check_power();
	print_editor_menu();
		sceCtrlReadBufferPositive(&pad, 1);  // press circle to escape

		if(pad.Buttons & PSP_CTRL_LTRIGGER) { // load file
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

		reset_ebuffer();
		print_editor_menu();
		for(t=0; t<259; t++) {
		ebuffercomp[t] = ebuffer[t];
		}

		for(fxncnt=0; fxncnt<9; fxncnt++) {
		sceDisplayWaitVblankStart();
		}

		read_labels();
		print_editor_menu();
		for(fxncnt=0; fxncnt<9; fxncnt++) {
		sceDisplayWaitVblankStart();
		}
		}

		if (pad.Buttons & PSP_CTRL_UP) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		ebuffer[cursor]--;


if (ascii == 0) {
		if (ebuffer[cursor] == 0x1F) {
		ebuffer[cursor] = 0x7E;
		}
}
if (ascii == 1) {
		if (ebuffer[cursor] == 0x00) {
		ebuffer[cursor] = 0x7E;
		}
}

		colcur = 5001;
		curspd++;
		print_editor_menu();

if (curspd > 6) {
			        for(fxncnt=0; fxncnt<4; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

		if (pad.Buttons & PSP_CTRL_DOWN) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		ebuffer[cursor]++;

if (ascii == 0) {
		if (ebuffer[cursor] == 0x7F) {
		ebuffer[cursor] = 0x20;
		}
}
if (ascii == 1) {
		if (ebuffer[cursor] == 0x7F) {
		ebuffer[cursor] = 0x01;
		}
}

		colcur = 5001;
		curspd++;
		print_editor_menu();


if (curspd > 6) {
			        for(fxncnt=0; fxncnt<4; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

		if (pad.Buttons & PSP_CTRL_LEFT) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor--;
		colcur = 5001;
		curspd++;
		print_editor_menu();

if (freespace == 1) {
		if (cursor == -1) {
		cursor = 235;
		}
}
if (freespace == 0) {
		if (cursor == -1) {
		cursor = 225;
		}
}

if (curspd > 6) {
			        for(fxncnt=0; fxncnt<1; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

		if (pad.Buttons & PSP_CTRL_RIGHT) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor++;
		colcur = 5001;
		curspd++;
		print_editor_menu();

if (freespace == 1) {
		if (cursor == 236) {
		cursor = 0;
		}
}
if (freespace == 0) {
		if (cursor == 226) {
		cursor = 0;
		}
}

if (curspd > 6) {
			        for(fxncnt=0; fxncnt<1; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

	sceCtrlSetSamplingMode(1); 
	sceCtrlReadBufferPositive(&pad, 1);

if(pad.Lx < 20) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor--;
		colcur = 5001;
		curspd++;
		print_editor_menu();
if (freespace == 1) {
		if (cursor == -1) {
		cursor = 235;
		}
}
if (freespace == 0) {
		if (cursor == -1) {
		cursor = 225;
		}
}

if (curspd > 6) {
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
}

if(pad.Lx > 235) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor++;
		colcur = 5001;
		curspd++;
		print_editor_menu();
if (freespace == 1) {
		if (cursor == 236) {
		cursor = 0;
		}
}
if (freespace == 0) {
		if (cursor == 226) {
		cursor = 0;
		}
}

if (curspd > 6) {
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
}

if(pad.Ly > 235) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		ebuffer[cursor]++;
if (ascii == 0) {
		if (ebuffer[cursor] == 0x7F) {
		ebuffer[cursor] = 0x20;
		}
}
if (ascii == 1) {
		if (ebuffer[cursor] == 0x7F) {
		ebuffer[cursor] = 0x01;
		}
}

		colcur = 5001;
		curspd++;
		print_editor_menu();


if (curspd > 6) {
			        for(fxncnt=0; fxncnt<3; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<8; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

if(pad.Ly < 20) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		ebuffer[cursor]--;


if (ascii == 0) {
		if (ebuffer[cursor] == 0x1F) {
		ebuffer[cursor] = 0x7E;
		}
}
if (ascii == 1) {
		if (ebuffer[cursor] == 0x00) {
		ebuffer[cursor] = 0x7E;
		}
}

		colcur = 5001;
		curspd++;
		print_editor_menu();

if (curspd > 6) {
			        for(fxncnt=0; fxncnt<3; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<8; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

		if (pad.Buttons & PSP_CTRL_CROSS) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
		reset_ebuffer();
		print_editor_menu();
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
for(t=0; t<259; t++) {
ebuffercomp[t] = ebuffer[t];
}
		}

		if (pad.Buttons & PSP_CTRL_SQUARE) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
if (dect == 0) {
		editor_read();
}
else {
		kws_editor_read();
}
		print_editor_menu();
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
for(t=0; t<259; t++) {
ebuffercomp[t] = ebuffer[t];
}
		}

		if (pad.Buttons & PSP_CTRL_TRIANGLE) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
		print_blankfooter();
		read_topmenu();
if (dect == 0) {
		editor_save();
}
else {
		kws_editor_save();
}
		txtsave();
		clear_midscreen();
		verify_topmenu();
		clear_midscreen();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00FFFFFF); //white
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	apply_settings();
	print_header();
		print_blankfooter();
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                ");
	pspDebugScreenSetTextColor(colord);
	printf("FLASH COMPLETE");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("                ;`-';");
	pspDebugScreenSetTextColor(colord);
		editor_footer();

for(t=0; t<259; t++) {
ebuffercomp[t] = ebuffer[t];
}
		}

		if (pad.Buttons & PSP_CTRL_CIRCLE) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		clear_midscreen();
		finger_ok_anim();
		for(fxncnt=0; fxncnt<90; fxncnt++) {
		sceDisplayWaitVblankStart();
		}
		clear_midscreen();
		sceDisplayWaitVblankStart();
		break;
		}

		if (pad.Buttons & PSP_CTRL_HOME) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample

	pspDebugScreenSetBackColor(0x00000000);

		clear_midscreen();
		break;
		}

		if (pad.Buttons & PSP_CTRL_START) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample

	pspDebugScreenSetBackColor(0x00000000);

		clear_midscreen();
		break;
		}

if (freespace == 1) {
if (colcur != 5237) {  // 5227
curspd = 0;
}
}

if (freespace == 0) {
if (colcur != 5227) {  // 5215
curspd = 0;
}
}

}
	} // seb
	else {
	sting = 1; // set unavailable in 2.71 SE message status
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	print_message();
	}



}
}

			if (menu == 8) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	blank_pmf();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	print_message();
			}
			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
if (seb == 0) {
	read_sysconf();
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
} // seb

if (oea == 1) {	//*****
read_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****


if (seb == 1) {
if (oea == 0) {
chkk = 1;
}
}


	if (chkk == 1) {

if (seb == 0) {

if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
}
else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
}
		sceIoWrite(fdo,data,133);
		sceIoClose(fdo);
		verify_index();

} // seb

if (seb == 1) {
if (oea == 0) {
sting = 1;
}
}

if (chkk == 1) {
if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}
}


				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();

	}
	if (chkk == 0) {
		print_blankfooter();

finger_stop_anim();

		pspDebugScreenSetTextColor(colord);
		pspDebugScreenSetXY(12, 28);
		printf(" REVERSE FIRMWARE VERSION 13.37 SPOOF FIRST ");
			        for(i=0; i<120; i++) {
		                sceDisplayWaitVblankStart();
				}
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
				print_footer();
			        for(i=0; i<100; i++) {
		                sceDisplayWaitVblankStart();
				}
	}

			        for(i=0; i<50; i++) {
		                sceDisplayWaitVblankStart();
				}

			}

			if (menu == 12) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
				write_file("./xflash/Registry/system.dreg","flash1:/registry/system.dreg");
				write_file("./xflash/Registry/system.ireg","flash1:/registry/system.ireg");
	if (exist == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	}


				print_regrestore();


if (exist == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
}


			}

			if (menu == 10) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

if (seb == 0) {

	read_sysconf();
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
	en1337();
	verify_sysconf();

if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
}
else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
}

		sceIoWrite(fdo,data,133);
		sceIoClose(fdo);
		verify_index();


} // seb

if (oea == 1) {	//*****
en1337_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****





if (seb == 1) {
if (oea == 0) {
sting = 1; // set unavailable message status
}
}

if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}



				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		data[cpa] = 49; // set menu default version tricker value 1.50 so menu isn't left on 3.37
		data[cpc] = 53;
		data[cpd] = 48;
		print_menu();
			}

			if (menu == 11) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	print_blankfooter();
	pspDebugScreenSetXY(18, 28);
	printf("       CHECK DUMP FOLDER      ");
	for(jibber=0; jibber<60; jibber++) {
	sceDisplayWaitVblankStart();
	}

	dumpfolderexists = 0;
	dfdx = sceIoDopen("./Xflash/Dump/");
	if(dfdx >= 0) {
	dumpfolderexists = 1;
	}
	rawdumpexists = 0;
	dfdx = sceIoDopen("./Xflash/Raw_Dump");
	if(dfdx >= 0) {
	rawdumpexists = 1;
	dumpfolderexists = 1;
	}

	pspDebugScreenSetXY(18, 28);
if (dumpfolderexists == 1) {
	printf("       DUMP FOLDER EXISTS     ");
}
else {
	print_blankfooter();
	pspDebugScreenSetXY(18, 28);

sceIoDevctl("ms0:", 0x02425818, &pbuf, sizeof(pbuf), 0, 0);
for (memd=0; memd<5; memd++);

freemem = buf[2] * buf[3] * buf[4];
freemem = freemem/1024;
freemem = freemem/1024;
	pspDebugScreenSetXY(18, 28);
pspDebugScreenPrintf(" MEMORY STICK HAS %d MB FREE",freemem);  

	for(jibber=0; jibber<130; jibber++) {
	sceDisplayWaitVblankStart();
	}

spreq = 16; // approx space required for 1.50 dump
if (seb == 1) {
spreq = 21; // approx space required for 2.71 SE dump
}
if (rawdump == 1) {
spreq = 32; // approx space required for raw dump of nand
}

if (freemem < spreq) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
	finger_stop_anim();
	print_blankfooter();
	pspDebugScreenSetXY(18, 28);
	printf("   NOT ENOUGH SPACE FOR DUMP  ");
	for(jibber=0; jibber<120; jibber++) {
	sceDisplayWaitVblankStart();
	}

goto skipdump;
}

	print_blankfooter();
	pspDebugScreenSetXY(18, 28);
	printf("       CREATE DUMP FOLDER     ");
}
	for(gbtrx=0; gbtrx<10; gbtrx++) {
	sceDisplayWaitVblankStart();
	}

	if (dumpfolderexists == 0) {
	sceIoMkdir("./Xflash/Dump", 0777);
	}

	if (rawdump == 1) {
	if (rawdumpexists == 0) {
	sceIoMkdir("./Xflash/Raw_Dump", 0777);
	}
	} // rawdump

	for(jibber=0; jibber<130; jibber++) {
	sceDisplayWaitVblankStart();
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);

	if (rawdump == 0) {
	pspDebugScreenSetXY(18, 27);
	printf("       DUMPING FLASH 0 ...");  
				dump_filesystem("flash0:/", "./Xflash/Dump/flash0");
	pspDebugScreenSetXY(18, 29);
	printf("       DUMPING FLASH 1 ...");  
				dump_filesystem("flash1:/", "./Xflash/Dump/flash1");
	} // rawdump
	if (rawdump == 1) {
	pspDebugScreenSetXY(18, 29);
	printf("       DUMPING LOWER FLASH");  
				dump_flashrom(1);
	pspDebugScreenSetXY(18, 27);
	printf("       DUMPING UPPER FLASH");
				dump_flashrom(0);
	} // rawdump

	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00FFFFFF); //white
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	print_menu();
	print_blankfooter();
	finger_ok_animB();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      FIRMWARE DUMP COMPLETE!");
			        for(i=0; i<100; i++) {
		                sceDisplayWaitVblankStart();
				}
	print_footer();
			}
skipdump:
				if (menu == 14) {


if (seb == 0) {

if (fms == 4) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample

	clear_midscreen();
	config_footer();
	more_configmenu_stuff();
while (1) {
	int fxncnt;

		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_DOWN) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		increment_configmenu();
		more_configmenu_stuff();
		}
		if (pad.Buttons & PSP_CTRL_UP) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		decrement_configmenu();
		more_configmenu_stuff();
		}

 		if(pad.Ly > 235) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		increment_configmenu();
		more_configmenu_stuff();
		}

		if(pad.Ly < 20) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		decrement_configmenu();
		more_configmenu_stuff();
		}

		if (pad.Buttons & PSP_CTRL_LEFT) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		do_configmenu_stuff();
		more_configmenu_stuff();
		}

		if (pad.Buttons & PSP_CTRL_RIGHT) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		do_configmenu_stuff();
		more_configmenu_stuff();
		}

 		if(pad.Lx > 235) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		do_configmenu_stuff();
		more_configmenu_stuff();
		}

		if(pad.Lx < 20) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		do_configmenu_stuff();
		more_configmenu_stuff();
		}

		if (pad.Buttons & PSP_CTRL_CROSS) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		do_configmenu_stuff();
		more_configmenu_stuff();
		}

		if (pad.Buttons & PSP_CTRL_TRIANGLE) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
		print_blankfooter();

		clear_midscreen();
		print_config_menu();

		configtoram();
		verify_config();

		more_configmenu_stuff();

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                ");
	pspDebugScreenSetTextColor(colord);
	printf("FLASH COMPLETE");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("                ;`-';");
	pspDebugScreenSetTextColor(colord);
		config_footer();
		}

		if (pad.Buttons & PSP_CTRL_SQUARE) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
		print_blankfooter();
		clear_midscreen();
		print_config_menu();
		delete_configuration();
		verify_configurationgone();
		more_configmenu_stuff();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                ");
	pspDebugScreenSetTextColor(colord);
	printf("FLASH COMPLETE");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("                ;`-';");
	pspDebugScreenSetTextColor(colord);
		config_footer();
		}

		if (pad.Buttons & PSP_CTRL_HOME) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample

	pspDebugScreenSetBackColor(0x00000000);

		clear_midscreen();
		break;
		}

		if (pad.Buttons & PSP_CTRL_START) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample

	pspDebugScreenSetBackColor(0x00000000);

		clear_midscreen();
		break;
		}

		if (pad.Buttons & PSP_CTRL_CIRCLE) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		clear_midscreen();
		print_config_menu();
		finger_ok_anim();
		print_config_menu();

				print_config_menu();
			        for(fxncnt=0; fxncnt<90; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}

		clear_midscreen();
		sceDisplayWaitVblankStart();
		break;
		}
}
}

			if (fms == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
				print_blankfooter();

if (test == 0) {

		write_file("flash0:/Song.mp3","ms0:/PSP/MUSIC/Song.mp3");
}
else {
		write_file("./Xflash/Dump/flash0/Song.mp3","ms0:/PSP/MUSIC/Song.mp3");
}

				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(17, 28);
				printf("           SONG RESTORED ");
			        for(i=0; i<100; i++) {
		                sceDisplayWaitVblankStart();
				}
			}

if (fms == 2) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
vshmaincheck();
if (exitdarkalex == 0) {
read_vshmain();
verify_vshmain();
vshfaketoram();
verify_vshfake();
recoverytoram();
verify_recovery();
configtoram();
verify_config();
}
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
}

if (fms == 3) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
vshmaincheckrev();
if (exitdarkalexrev == 0) {
read_vshmainrev();
verify_vshmainrev();
delete_vshmainreal();
verify_vshmainrealgone();
delete_recovery();
verify_recoverygone();
delete_configuration();
verify_configurationgone();
}
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
}

			if (fms == 1) {
			print_justfooter();
			print_delete_song();
			delete_song();
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
			}
				print_footer();
				pspDebugScreenSetXY(0, 24);
				printf("                               ");

if (fms != 2) {
if (fms != 3) {
if (fms != 4) {
if (fms != 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
				print_message();
}
}
}
}

if (fms == 2) {
if (exitdarkalex == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
				print_message();
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
}
}

if (fms == 3) {
if (exitdarkalexrev == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
				print_message();
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
}
}


	} // seb
if (seb == 1) {
if (oea == 0) {
if (fms != 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	sting = 1;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
				print_message();
	} // fms
	} // seb
	} // oea
if (seb == 1) {
if (oea == 1) {
if (fms != 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	sting = 1;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
				print_message();
	} // fms
	} // seb
	} // oea

if (fms == 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
print_information();
}

				}

				if (menu == 13) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
				print_blankfooter();
				save_settings();
			        for(i=0; i<50; i++) {
		                sceDisplayWaitVblankStart();
				}
				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(17, 28);
				printf("          SETTINGS SAVED ");
				hilight = colord;
				flash_title();
			        for(i=0; i<90; i++) {
		                sceDisplayWaitVblankStart();
				}
				print_footer();
				}
				pspDebugScreenSetXY(0, 24);
				printf("                               ");
				}

		if (pad.Buttons & PSP_CTRL_CIRCLE) {
		grey = 0x00555555;

			if (menu == 15) {
if (qpr == 2) {
	if (test == 0) {
	//voice = Sample_Play(ch,0,0);
	//Voice_SetVolume(voice, vol);
	//Voice_SetPanning(voice, pan);

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 1; // select button press sample

	dumpfolderexists = 0;
	color = 0x00008D8F; //test mode theme
	colorb = 0x00008D8F;
	colorc = 0x00333333;
	colord = 0x00008D8F;
	print_header();
	print_justfooter();
	draw_chicken();
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("       CHECK DUMP FOLDER      ");
	for(jibber=0; jibber<60; jibber++) {
	sceDisplayWaitVblankStart();
	}

	dfdx = sceIoDopen("./Xflash/Dump/flash0");
	if(dfdx >= 0) {
	dumpfolderexists = 1;
	}

if (dumpfolderexists == 0) {

	apply_settings();
	print_header();
print_menu();
	print_justfooter();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
saynos++;
if (saynos > 5) {	// counter for flipping the finger easter egg
flip_finger();
}
else {
finger_stop_anim();
}

	print_blankfooter();
	pspDebugScreenSetXY(18, 27);

if (saynos > 5) {
	printf("     FLIPPING THE FINGER !!   ");
}
else {
	printf("                              ");
}

	pspDebugScreenSetXY(18, 28);
	printf("  PERFORM FIRMWARE DUMP FIRST ");



	for(jibber=0; jibber<120; jibber++) {
	sceDisplayWaitVblankStart();
	}
}
else {
another_function();
}

	}
	else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	test = 0;	//reset test mode status
	apply_settings();
	print_header();
	print_justfooter();
			        for(i=0; i<11; i++) {
		                sceDisplayWaitVblankStart();
				}
	}
}

if (qpr == 0) {
fxt = 0;
		print_blankfooter();
		finger_patch();
while (1)	   {
		   sceCtrlReadBufferPositive(&pad, 1);
		   if (pad.Buttons & PSP_CTRL_CIRCLE) {
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
			clear_midscreen();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 17);
	printf("         PROGRAM EXIT      ");
fxt = 0;
while (1)	   {
		SceCtrlData sleep;				// 'pad' buffer for standby hold button detect
		sceCtrlReadBufferPositive(&sleep, 1);		// detect extra long button hold
		if (sleep.Buttons & PSP_CTRL_CIRCLE) {
if (barpatch == 1) {
		pspDebugScreenSetTextColor(0x00000000);		// black (cover up buggy bar)
}
else {
		if (settings[0x64] == 0x3E) {
		pspDebugScreenSetTextColor(2109782);		// metallic bronze colour
		}
		else {
		pspDebugScreenSetTextColor(0x001111FE);		// red for standby slider bar
		}
}
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
		print_blankfooter();
			clear_midscreen();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 17);
	printf("           PSP OFF         ");
fxt = 0;
barpatch = 1;	// set standby status for black bar patch
			int scePowerRequestStandby(void);
			scePowerRequestStandby();
			}
}
			print_blankfooter();
    pspAudioEnd();
			sceKernelExitGame();
			return 0;
			}
}
}
			}

			if (menu == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample

			if (dzm == 0) {
			read_opening();
			enbrandop();
			verify_opening();
			}

			if (dzm == 1) {
			read_opening();
			enbrandgb();
			verify_opening();
			}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}
			if (menu == 2) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
if (oea == 0) {
			read_system();	//restore original error and click sounds
			org_error();
			org_click();
			verify_system();
}
			read_opening();
			delete_opening_vag();	//restore original opening sound
			original_opening();
			verify_opening();

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}

			if (menu == 3) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
			read_update();
			enable_update();
			verify_update();
	if (seb == 0) {
			read_topmenu();
			txtenabled();
			verify_topmenu();
	}
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}

			if (menu == 4) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
			read_sysbg();
			enable_wave();
			verify_sysbg();
			read_opening();
			enwaveop();
			verify_opening();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}

			if (menu == 5) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
if (month == 1) {
if (oea == 0) {
jantoram();
verify_jan();
}
if (oea == 1) {
alltoram();
verify_all();
}
}
if (month == 2) {
febtoram();
verify_feb();
}
if (month == 3) {
martoram();
verify_mar();
}
if (month == 4) {
aprtoram();
verify_apr();
}
if (month == 5) {
maytoram();
verify_may();
}
if (month == 6) {
juntoram();
verify_jun();
}
if (month == 7) {
jultoram();
verify_jul();
}
if (month == 8) {
augtoram();
verify_aug();
}
if (month == 9) {
septoram();
verify_sep();
}
if (month == 10) {
octtoram();
verify_oct();
}
if (month == 11) {
novtoram();
verify_nov();
}
if (month == 12) {
dectoram();
verify_dec();
}
if (month == 13) {

if (oea == 0) {
jantoram();
verify_jan();
febtoram();
verify_feb();
martoram();
verify_mar();
aprtoram();
verify_apr();
maytoram();
verify_may();
juntoram();
verify_jun();
jultoram();
verify_jul();
augtoram();
verify_aug();
septoram();
verify_sep();
octtoram();
verify_oct();
novtoram();
verify_nov();
dectoram();
verify_dec();
} // oea
if (oea == 1) {
alltoram();
verify_all();
}

}
if (month == 14) {

if (oea == 0) {
jantoram();
verify_jan();
febtoram();
verify_feb();
martoram();
verify_mar();
aprtoram();
verify_apr();
maytoram();
verify_may();
juntoram();
verify_jun();
jultoram();
verify_jul();
augtoram();
verify_aug();
septoram();
verify_sep();
octtoram();
verify_oct();
novtoram();
verify_nov();
dectoram();
verify_dec();
} // oea
if (oea == 1) {
alltoram();
verify_all();
}

}
if (month == 15) {

if (oea == 0) {
jantoram();
verify_jan();
febtoram();
verify_feb();
martoram();
verify_mar();
aprtoram();
verify_apr();
maytoram();
verify_may();
juntoram();
verify_jun();
jultoram();
verify_jul();
augtoram();
verify_aug();
septoram();
verify_sep();
octtoram();
verify_oct();
novtoram();
verify_nov();
dectoram();
verify_dec();
} // oea
if (oea == 1) {
alltoram();
verify_all();
}

}

if (month == 16) {
delete_name_plate();
verify_name_plate_gone();
}

if (month == 17) {


	if (seb == 0) {

read_sysconf();
xoasp();
verify_sysconf();
	} // seb
	else {
	sting = 1;
	}
}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}
			if (menu == 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
			fonttoram();
			verify_font();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}
			if (menu == 7) {
if (game != 19) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
	if (seb == 0) {
			read_topmenu();

			if (game == 18) {
			retmtm();
			verify_topmenu();
			read_sysconf();
			retmsc();
			verify_sysconf();
			}

			else {
			if (game < 10) {
			txtgame();
			}
			if (game > 9) {
			if (game < 13) {
			txtmusic();
			}
			}
			if (game > 12) {
			if (game < 16) {
			txtphoto();
			}
			}
			if (game > 15) {
			txtvideo();
			}

			verify_topmenu();
			}

	} // seb
	else {
	sting = 1; // set unavailable in 2.71 SE message status
	}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}
}

			if (menu == 8) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
			gameboottoram();
			verify_gameboot();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_message();
			}

			if (menu == 9) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
	if (seb == 0) {
			read_sysconf();
			print_blankfooter();
			pspDebugScreenSetTextColor(colord);
			pspDebugScreenSetXY(18, 28);
			printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
			de1337();
			verify_sysconf();

if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
}
else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
}

		sceIoWrite(fdo,data,133);
		sceIoClose(fdo);
		verify_index();
	} // seb


if (oea == 1) {	//*****
orig_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
			printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****

	if (seb == 1) {
	if (oea == 0) {
	sting = 1;
	}
	}

if (chkk == 1) {
if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}
}
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
			}


			if (menu == 12) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
	print_blankfooter();
	pspDebugScreenSetXY(18, 28);
	printf("     CHECK REGISTRY FOLDER    ");
	for(jibber=0; jibber<60; jibber++) {
	sceDisplayWaitVblankStart();
	}

	dfdx = sceIoDopen("./Xflash/Registry/");
	if(dfdx >= 0) {
	registryfolderexists = 1;
	}

	pspDebugScreenSetXY(18, 28);
if (registryfolderexists == 1) {
	printf("     REGISTRY FOLDER EXISTS   ");
}
else {
	printf("     CREATE REGISTRY FOLDER   ");
}
	for(gbtrx=0; gbtrx<10; gbtrx++) {
	sceDisplayWaitVblankStart();
	}

if (registryfolderexists == 0) {
sceIoMkdir("./Xflash/Registry", 0777);
}
	for(jibber=0; jibber<130; jibber++) {
	sceDisplayWaitVblankStart();
	}

			write_file("flash1:/registry/system.dreg","./xflash/Registry/system.dreg");
			write_file("flash1:/registry/system.ireg","./xflash/Registry/system.ireg");
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
			print_regmessage();
			}


			if (menu == 10) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 2; // select button press sample
if (seb == 0) {
	read_sysconf();
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
			de1337();
			verify_sysconf();

if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
}
else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
}

		sceIoWrite(fdo,data,133);
		sceIoClose(fdo);
		verify_index();
	} // seb

if (oea == 1) {	//*****
orig_sysctrl();	//*****
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
			printf(" REMOVE FIRMWARE VERSION SPOOF");
			for(i=0; i<60; i++) {
		        sceDisplayWaitVblankStart();
			}
}		//*****


	if (seb == 1) {
	if (oea == 0) {
	sting = 1;
	}
	}

if (seb == 1) {
if (oea == 1) {
write_sysctrl();
for(i=0; i<30; i++) {
sceDisplayWaitVblankStart();
}
verify_sysctl();
}
}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		}


			if (menu == 14) {

			if (fms == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	if (seb == 0) {
if (test == 0) {
		write_file("ms0:/PSP/MUSIC/Song.mp3","flash0:/Song.mp3");
}
if (test != 0) {
		write_file("ms0:/PSP/MUSIC/Song.mp3","./Xflash/Dump/flash0/Song.mp3");
}

	} // seb
	if (seb == 1) {
	sting = 1;
	}


				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
				}

}
		}

	if (pad.Buttons & PSP_CTRL_TRIANGLE) {
	if (menu == 8) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
	read_gameboot_file();
	verify_user_gameboot();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	print_message();
	}
	}

		if(sceHprmIsRemoteExist()  ){
		u32 keya;
		int errb;
		errb = sceHprmPeekCurrentKey(&keya);
		if(keya == PSP_HPRM_FORWARD) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample
		if (menu > 0) {
		menu--;
		for(i=0; i<9; i++) {
		sceDisplayWaitVblankStart();
		}
          	}
		if (menu == 0) {
		menu = 15;
		}
		scroll_dn();
		}
		if(keya == PSP_HPRM_BACK) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample
		if (menu < 16) {
		menu++;
		for(i=0; i<9; i++) {
		sceDisplayWaitVblankStart();
		}
		}
		if (menu == 16) {
		menu = 1;
		}
		scroll_up();
		}
		}

if (dishome == 0) {
		if (pad.Buttons & PSP_CTRL_HOME) {
		fxt = 0;
		print_blankfooter();
		while (menu != 15) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample
		menu++;
		scroll_up();
		print_menu();
		for(i=0; i<1; i++) {	// adjust speed of auto scrolling to exit option
		sceDisplayWaitVblankStart();
		}
		}
		print_blankfooter();
		while (qpr != 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		increment_qpr();
		print_menu();
		for(i=0; i<1; i++) {
		sceDisplayWaitVblankStart();
		}
		}
		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_HOME) {
		print_blankfooter();
		finger_patch();
		}
while (1)	   {
		   sceCtrlReadBufferPositive(&pad, 1);
		   if (pad.Buttons & PSP_CTRL_HOME) {
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
			clear_midscreen();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 17);
	printf("         PROGRAM EXIT      ");
fxt = 0;
while (1)	   {
		SceCtrlData sleep;				// 'pad' buffer for standby hold button detect
		sceCtrlReadBufferPositive(&sleep, 1);		// detect extra long button hold
		if (sleep.Buttons & PSP_CTRL_HOME) {
if (barpatch == 1) {
		pspDebugScreenSetTextColor(0x00000000);		// black (cover up buggy bar)
}
else {
		if (settings[0x64] == 0x3E) {
		pspDebugScreenSetTextColor(2109782);		// metallic bronze colour
		}
		else {
		pspDebugScreenSetTextColor(0x001111FE);		// red for standby slider bar
		}
}
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
		print_blankfooter();
			clear_midscreen();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 17);
	printf("           PSP OFF         ");
fxt = 0;
barpatch = 1;	// set standby status for black bar patch
			int scePowerRequestStandby(void);
			scePowerRequestStandby();
			}
}
			print_blankfooter();
    pspAudioEnd();
			sceKernelExitGame();
			return 0;
		}
		}
		}
}

if (disstart == 0) {
		if (pad.Buttons & PSP_CTRL_START) {
		fxt = 0;
		print_blankfooter();
		while (menu != 15) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample
		menu++;
		scroll_up();
		print_menu();
		for(i=0; i<1; i++) {
		sceDisplayWaitVblankStart();
		}
		}
		print_blankfooter();
		while (qpr != 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		increment_qpr();
		print_menu();
		for(i=0; i<1; i++) {
		sceDisplayWaitVblankStart();
		}
		}
		sceCtrlReadBufferPositive(&pad, 1);
		if (pad.Buttons & PSP_CTRL_START) {
		print_blankfooter();
		finger_patch();
		}
while (1)	   {
		   sceCtrlReadBufferPositive(&pad, 1);
		   if (pad.Buttons & PSP_CTRL_START) {
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
			clear_midscreen();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 17);
	printf("         PROGRAM EXIT      ");
fxt = 0;
while (1)	   {
		SceCtrlData sleep;				// 'pad' buffer for standby hold button detect
		sceCtrlReadBufferPositive(&sleep, 1);		// detect extra long button hold
		if (sleep.Buttons & PSP_CTRL_START) {
if (barpatch == 1) {
		pspDebugScreenSetTextColor(0x00000000);		// black (cover up buggy bar)
}
else {
		if (settings[0x64] == 0x3E) {
		pspDebugScreenSetTextColor(2109782);		// metallic bronze colour
		}
		else {
		pspDebugScreenSetTextColor(0x001111FE);		// red for standby slider bar
		}
}
		   pspDebugScreenSetXY(fxt + 11, 28);
		   printf("X");
if (fxt != 0) {
		   pspDebugScreenSetXY(fxt + 10, 27);
		   printf("X");
		   pspDebugScreenSetXY(fxt + 10, 29);
		   printf("X");
		}
		   fxt++;
		      sceDisplayWaitVblankStart();
		   }
		   else {
		   break;
		   }
if (fxt == 46) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 11; // select button press sample
		print_blankfooter();
			clear_midscreen();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 17);
	printf("           PSP OFF         ");
fxt = 0;
barpatch = 1;	// set standby status for black bar patch
			int scePowerRequestStandby(void);
			scePowerRequestStandby();
			}
}
			print_blankfooter();
    pspAudioEnd();
			sceKernelExitGame();
			return 0;
		}
		}
		}
}

if (disselect == 0) {
		if (pad.Buttons & PSP_CTRL_SELECT) {
		fxt = 0;
		print_blankfooter();
		while (menu != 7) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 7; // select button press sample

		if (menu < 16) {
		menu++;
		}
		if (menu == 16) {
		menu = 1;
		}
		scroll_up();
		print_menu();
		for(i=0; i<1; i++) {
		sceDisplayWaitVblankStart();
		}
		}
		print_blankfooter();
		while (game != 19) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		increment_game();
		print_menu();
		for(i=0; i<1; i++) {
		sceDisplayWaitVblankStart();
		}
		}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
editxmb = 0;

if (seb == 0) { // allow menu editor with 1.50 firmware
editxmb = 1;
}

if (dect == 1) { // allow menu editor with decrypted 2.50 topmenu_plugin.rco
editxmb = 1;
}

if (editxmb == 1) {
	clear_midscreen();
	read_topmenu();
	reset_ebuffer();
	reset_ebuffercomp();
	print_editor_menu();
	editor_footer();
	print_editor_menu();

	for(i=0; i<10; i++) {
	sceDisplayWaitVblankStart();
	}


while (1) {
	int fxncnt;
	check_power();
	print_editor_menu();
		sceCtrlReadBufferPositive(&pad, 1);  // press circle to escape

		if(pad.Buttons & PSP_CTRL_LTRIGGER) { // load file
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample

		reset_ebuffer();
		print_editor_menu();

		for(t=0; t<259; t++) {
		ebuffercomp[t] = ebuffer[t];
		}

		for(fxncnt=0; fxncnt<9; fxncnt++) {
		sceDisplayWaitVblankStart();
		}

		read_labels();
		print_editor_menu();
		for(fxncnt=0; fxncnt<9; fxncnt++) {
		sceDisplayWaitVblankStart();
		}
		}

		if (pad.Buttons & PSP_CTRL_UP) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		ebuffer[cursor]--;

if (ascii == 0) {
		if (ebuffer[cursor] == 0x1F) {
		ebuffer[cursor] = 0x7E;
		}
}
if (ascii == 1) {
		if (ebuffer[cursor] == 0x00) {
		ebuffer[cursor] = 0x7E;
		}
}

		colcur = 5001;
		curspd++;
		print_editor_menu();

if (curspd > 6) {
			        for(fxncnt=0; fxncnt<4; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

		if (pad.Buttons & PSP_CTRL_DOWN) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		ebuffer[cursor]++;

if (ascii == 0) {
		if (ebuffer[cursor] == 0x7F) {
		ebuffer[cursor] = 0x20;
		}
}
if (ascii == 1) {
		if (ebuffer[cursor] == 0x7F) {
		ebuffer[cursor] = 0x01;
		}
}

		colcur = 5001;
		curspd++;
		print_editor_menu();


if (curspd > 6) {
			        for(fxncnt=0; fxncnt<4; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

		if (pad.Buttons & PSP_CTRL_LEFT) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor--;
		colcur = 5001;
		curspd++;
		print_editor_menu();

if (freespace == 1) {
		if (cursor == -1) {
		cursor = 235;
		}
}
if (freespace == 0) {
		if (cursor == -1) {
		cursor = 225;
		}
}

if (curspd > 6) {
			        for(fxncnt=0; fxncnt<1; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

		if (pad.Buttons & PSP_CTRL_RIGHT) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor++;
		colcur = 5001;
		curspd++;
		print_editor_menu();

if (freespace == 1) {
		if (cursor == 236) {
		cursor = 0;
		}
}
if (freespace == 0) {
		if (cursor == 226) {
		cursor = 0;
		}
}

if (curspd > 6) {
			        for(fxncnt=0; fxncnt<1; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

	sceCtrlSetSamplingMode(1); 
	sceCtrlReadBufferPositive(&pad, 1);

if(pad.Lx < 20) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor--;
		colcur = 5001;
		curspd++;
		print_editor_menu();
if (freespace == 1) {
		if (cursor == -1) {
		cursor = 235;
		}
}
if (freespace == 0) {
		if (cursor == -1) {
		cursor = 225;
		}
}

if (curspd > 6) {
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
}

if(pad.Lx > 235) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor++;
		colcur = 5001;
		curspd++;
		print_editor_menu();
if (freespace == 1) {
		if (cursor == 236) {
		cursor = 0;
		}
}
if (freespace == 0) {
		if (cursor == 226) {
		cursor = 0;
		}
}

if (curspd > 6) {
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
}

if(pad.Lx > 235) {
if (curspd > 6) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
else {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
}
		cursor++;
		colcur = 5001;
		curspd++;
		print_editor_menu();

		if (cursor == 236) {
		cursor = 0;
		}

if (curspd > 6) {
}
else {
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
}

   if(pad.Ly > 235) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		ebuffer[cursor]++;

if (ascii == 0) {
		if (ebuffer[cursor] == 0x7F) {
		ebuffer[cursor] = 0x20;
		}
}
if (ascii == 1) {
		if (ebuffer[cursor] == 0x7F) {
		ebuffer[cursor] = 0x01;
		}
}

		colcur = 5001;
		curspd++;
		print_editor_menu();


if (curspd > 6) {
			        for(fxncnt=0; fxncnt<3; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<8; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}

   if(pad.Ly < 20) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 8; // select button press sample
		ebuffer[cursor]--;


if (ascii == 0) {
		if (ebuffer[cursor] == 0x1F) {
		ebuffer[cursor] = 0x7E;
		}
}
if (ascii == 1) {
		if (ebuffer[cursor] == 0x00) {
		ebuffer[cursor] = 0x7E;
		}
}
		colcur = 5001;
		curspd++;
		print_editor_menu();

if (curspd > 6) {
			        for(fxncnt=0; fxncnt<3; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
else {
			        for(fxncnt=0; fxncnt<8; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
}
		}
		if (pad.Buttons & PSP_CTRL_CROSS) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
		reset_ebuffer();
		print_editor_menu();
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
for(t=0; t<259; t++) {
ebuffercomp[t] = ebuffer[t];
}
		}

		if (pad.Buttons & PSP_CTRL_SQUARE) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 3; // select button press sample
if (dect == 0) {
		editor_read();
}
else {
		kws_editor_read();
}
		print_editor_menu();
			        for(fxncnt=0; fxncnt<9; fxncnt++) {
		                sceDisplayWaitVblankStart();
				}
for(t=0; t<259; t++) {
ebuffercomp[t] = ebuffer[t];
}
		}

		if (pad.Buttons & PSP_CTRL_TRIANGLE) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 5; // select button press sample
		print_blankfooter();
		read_topmenu();
if (dect == 0) {
		editor_save();
}
else {
		kws_editor_save();
}
		txtsave();
		clear_midscreen();
		verify_topmenu();
		clear_midscreen();
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00FFFFFF); //white
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	apply_settings();
	print_header();
	print_blankfooter();
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                ");
	pspDebugScreenSetTextColor(colord);
	printf("FLASH COMPLETE");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("                ;`-';");
	pspDebugScreenSetTextColor(colord);
		editor_footer();


for(t=0; t<259; t++) {
ebuffercomp[t] = ebuffer[t];
}
		}

		if (pad.Buttons & PSP_CTRL_CIRCLE) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		clear_midscreen();
		finger_ok_anim();
		for(fxncnt=0; fxncnt<90; fxncnt++) {
		sceDisplayWaitVblankStart();
		}
		clear_midscreen();
		sceDisplayWaitVblankStart();
		break;
		}

		if (pad.Buttons & PSP_CTRL_HOME) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		clear_midscreen();
		break;
		}

		if (pad.Buttons & PSP_CTRL_START) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		clear_midscreen();
		break;
		}

		if (pad.Buttons & PSP_CTRL_SELECT) {
		grey = 0x00555555;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		clear_midscreen();
		finger_ok_anim();
		for(fxncnt=0; fxncnt<90; fxncnt++) {
		sceDisplayWaitVblankStart();
		}

		clear_midscreen();
		sceDisplayWaitVblankStart();
		break;
		}

if (freespace == 1) {
if (colcur != 5237) {  // 5227
curspd = 0;
}
}

if (freespace == 0) {
if (colcur != 5227) {  // 5215
curspd = 0;
}
}

}


} // seb
		if (seb == 1) {
		if (oea == 0) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		sting = 1;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		}
		}

		if (dect == 0) {
		if (oea == 1) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		sting = 1;
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select button press sample
		print_message();
		}
		}


}
}

	if (greydir == 0) {
	grey = grey + 0x00111111;
	if (grey == 0x00FFFFFF) {
	greydir = 1;
	}
	}
	else {
	grey = grey - 0x00111111;
	if (grey == 0x00111111) {
	greydir = 0;
	}
	}
	print_menu();

	}
    pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}

  // allow audio thread to terminate cleanly
  done = true;
  if (mikModThreadID > 0)
  {
    SceUInt timeout = 100000;
    sceKernelWaitThreadEnd(mikModThreadID, &timeout);
    // not 100% sure if this is necessary after a clean exit, but just to make sure any resources are freed:
    sceKernelDeleteThread(mikModThreadID);
  }
	Player_Stop();
	Player_Free(mf);
	MikMod_Exit();
    pspAudioEnd();
	sceKernelExitGame();
	return 0;
}

void check(const char* zFile){

	int fd3;
	fd3 = sceIoOpen(zFile, PSP_O_RDONLY, 0);
	if(fd3 < 0) {
		exist = 0;
        }
	else {
                exist = 1;
        }
	sceIoClose(fd3);
} 

void write_file(const char *readpath, const char *writepath) {
	check(readpath);
	if(exist == 1) {
		int fdin;
		int fdout;	

		fdin = sceIoOpen(readpath, PSP_O_RDONLY, 0777);
		if(fdin >= 0)
		{
			int bytesRead = 1;
			fdout = sceIoOpen(writepath, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
			bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
			while((bytesRead > 0) && (fdout >= 0))
			{
				sceIoWrite(fdout, write_buffer, bytesRead);
				bytesRead = sceIoRead(fdin, write_buffer, sizeof(write_buffer));
			}

			if(fdout >= 0)
			{
				sceIoClose(fdout);
			}	

			if(fdin >= 0)
			{
				sceIoClose(fdin);
			}
		}
	}
}

void print_menu() {

if (lcol > 0) {
lcol--;
}

	check_power();

	pspDebugScreenSetXY(lcol, mlinea);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinea;
		dither();

if (fms == 0) {
	printf("                  14 COPY AN MP3 FILE TO FLASH MEMORY    ");
}
if (fms == 1) {
	printf("                  14 DELETE MP3 FILE FROM FLASH MEMORY   ");
}
if (fms == 2) {
	printf("                  14 APPLY DAX CUSTOM FIRMWARE FILES     ");
}
if (fms == 3) {
	printf("                  14 REMOVE DAX CUSTOM FIRMWARE FILES    ");
}
if (fms == 4) {
	printf("                  14 OPEN CUSTOM FIRMWARE CONFIGURATOR   ");
}
if (fms == 5) {
	printf("                  14 OPEN PSP UNIT INFORMATION SCREEN    ");
}

	pspDebugScreenSetXY(lcol, mlineo);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlineo;
		dither();

if (qpr == 0) {
	printf("                  15 QUIT X-FLASH PROGRAM                 ");
}
if (qpr == 1) {
	printf("                  15 AUTO RESTORE AND QUIT PROGRAM        ");
}
if (qpr == 2) {
if (test == 0) {
	printf("                  15 START SIMULATION MODE                ");
}
else {
	printf("                  15 EXIT SIMULATION MODE                 ");
}
}
if (qpr == 3) {
	printf("                  15 READ EULA AND DOCUMENTATION          ");
}
if (qpr == 4) {
	printf("                  15 START ASCII GRAPHICS CREDITS DEMO    ");
}
if (qpr == 5) {
	printf("                  15 PLAY FREE SPACE GAME                 ");
}


	pspDebugScreenSetXY(lcol, mlineb);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlineb;
		dither();

	printf("                   8 CHANGE GAME BOOT INTRO MOVIE         ");
	pspDebugScreenSetXY(lcol, mlinec);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinec;
		dither();

	printf("                   9 APPLY FIRMWARE VERSION ");
	printf("%c",data[cpa]);
	printf("%c",data[cpb]);
	printf("%c",data[cpc]);
	printf("%c",data[cpd]);
	printf(" SPOOF  ");

	pspDebugScreenSetXY(lcol, mlined);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlined;
		dither();

	printf("                  10 APPLY FIRMWARE VERSION 13.37 SPOOF  ");
	pspDebugScreenSetXY(lcol, mlinee);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinee;
		dither();
if (rawdump == 0) {
	printf("                  11 PERFORM COMPLETE FIRMWARE DUMP     ");
} else {
	printf("                  11 PERFORM COMPLETE RAW FLASH DUMP    ");
} // rawdump
	pspDebugScreenSetXY(lcol, mlinef);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinef;
		dither();

	printf("                  12 PERFORM REGISTRY RESTORE / BACKUP  ");
	pspDebugScreenSetXY(lcol, mlineg);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlineg;
		dither();

	printf("                  13 CHANGE X-FLASH MENU THEME          ");
	pspDebugScreenSetXY(lcol, mlineh);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlineh;
		dither();

if (dzm == 0) {
	printf("                   1 APPLY DEBRANDED INTRO SCREEN       ");
}
if (dzm == 1) {
	printf("                   1 APPLY DEBRANDED GAMEBOOT SCREEN    ");
}


	pspDebugScreenSetXY(lcol, mlinei);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinei;
		dither();

	printf("                   2 APPLY ALTERNATE SOUND EFFECT       ");
	pspDebugScreenSetXY(lcol, mlinej);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinej;
		dither();

	printf("                   3 DISABLE NETWORK UPDATE ICON         ");
	pspDebugScreenSetXY(lcol, mlinek);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinek;
		dither();

	printf("                   4 REMOVE XMB MENU WAVE EFFECT         ");
	pspDebugScreenSetXY(lcol, mlinel);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinel;
		dither();

if (month == 1) {
if (oea == 0) {
	printf("                   5 REPLACE JANUARY BACKGROUND         ");
}
if (oea == 1) {
	printf("                   5 REPLACE BACKGROUND PICTURES        ");
}
}
if (month == 2) {
	printf("                   5 REPLACE FEBRUARY BACKGROUND        ");
}
if (month == 3) {
	printf("                   5 REPLACE MARCH BACKGROUND           ");
}
if (month == 4) {
	printf("                   5 REPLACE APRIL BACKGROUND           ");
}
if (month == 5) {
	printf("                   5 REPLACE MAY BACKGROUND             ");
}
if (month == 6) {
	printf("                   5 REPLACE JUNE BACKGROUND            ");
}
if (month == 7) {
	printf("                   5 REPLACE JULY BACKGROUND            ");
}
if (month == 8) {
	printf("                   5 REPLACE AUGUST BACKGROUND          ");
}
if (month == 9) {
	printf("                   5 REPLACE SEPTEMBER BACKGROUND       ");
}
if (month == 10) {
	printf("                   5 REPLACE OCTOBER BACKGROUND         ");
}
if (month == 11) {
	printf("                   5 REPLACE NOVEMBER BACKGROUND        ");
}
if (month == 12) {
	printf("                   5 REPLACE DECEMBER BACKGROUND        ");
}
if (month == 13) {
	printf("                   5 REPLACE EVERY BACKGROUND PICTURE   ");
}
if (month == 14) {
	printf("                   5 APPLY PLAIN BLACK BACKGROUNDS      ");
}
if (month == 15) {
	printf("                   5 RESTORE ALL ORIGINAL BACKGROUNDS   ");
}
if (month == 16) {
	printf("                   5 APPLY CUSTOM NAME PLATE PICTURE    ");
}
if (month == 17) {
	printf("                   5 APPLY PLAIN BLACK ABOUT SCREENS    ");
}

	pspDebugScreenSetXY(lcol, mlinem);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinem;
		dither();

	printf("                   6 INSTALL CUSTOM FONT                ");
	pspDebugScreenSetXY(lcol, mlinen);
		pspDebugScreenSetTextColor(colorc);
		fetch = mlinen;
		dither();

if (game == 19) {
	printf("                   7 START SEVEN HOUR XMB MENU EDITOR   ");
}

if (game == 18) {
	printf("                   7 REMOVE ALL XMB TRADEMARK SYMBOLS   ");
}
else {
if (game != 19) {
	printf("                   7 APPLY CUSTOM ");

if (game < 10) {
	printf("GAME MENU LABEL       ");
}
if (game > 9) {
if (game < 13) {
	printf("MUSIC MENU LABEL      ");
}
}
if (game > 12) {
if (game < 16) {
	printf("PHOTO MENU LABEL      ");
}
}
if (game > 15) {
	printf("VIDEO MENU LABEL      ");
}
}
}

	pspDebugScreenSetTextColor(grey); //flashing grey
//	pspDebugScreenSetXY(13, 17);
//	printf("==>");

	pspDebugScreenSetXY(6, 15);
	printf("  ____");
	pspDebugScreenSetXY(6, 16);
	printf("-'  __\\____");
	pspDebugScreenSetXY(6, 17);
	printf("       ____)");
	pspDebugScreenSetXY(6, 18);
	printf("       _)");
	pspDebugScreenSetXY(6, 19);
	printf("      _)");
	pspDebugScreenSetXY(6, 20);
	printf("-.____)");

		if (mlineg == 17) {
	pspDebugScreenSetXY(55, mlineg);
		move_arrow();
		}
		if (mlinei == 17) {
	pspDebugScreenSetXY(55, mlinei);
		move_arrow();
		}
		if (mlinen == 17) {
	pspDebugScreenSetXY(55, mlinen);
		move_arrow();
		}
		if (mlinec == 17) {
	pspDebugScreenSetXY(55, mlinec);
		move_arrow();
		}
		if (mlineh == 17) {
	pspDebugScreenSetXY(55, mlineh);
		move_arrow();
		}
		if (mlinel == 17) {
	pspDebugScreenSetXY(55, mlinel);
		move_arrow();
		}
		if (mlineo == 17) {
	pspDebugScreenSetXY(55, mlineo);
		move_arrow();
		}
		if (mlinea == 17) {
	pspDebugScreenSetXY(55, mlinea);
		move_arrow();
		}


		if (mlinea != 17) {
		if (mlineh != 17) {
		if (mlinel != 17) {
		if (mlineg != 17) {
		if (mlinec != 17) {
		if (mlinen != 17) {
		if (mlinei != 17) {
		if (mlineo != 17) {
	pspDebugScreenSetXY(55, 17);
			printf("      ");
		}
		}
		}
		}
		}
		}
		}
		}

	print_header();
	print_footer();
}

void move_arrow() {

if (grey < 0x002AAAAA) {
	pspDebugScreenSetXY(6, 15); // halfway extended
	printf(" ____ ");
	pspDebugScreenSetXY(6, 16);
	printf("'  __\\____ ");
	pspDebugScreenSetXY(6, 17);
	printf("      ____) ");
	pspDebugScreenSetXY(6, 18);
	printf("      _) ");
	pspDebugScreenSetXY(6, 19);
	printf("     _) ");
	pspDebugScreenSetXY(6, 20);
	printf(".____) ");
}
if (grey < 0x00555554) {
if (grey > 0x002AAAAA) {
	pspDebugScreenSetXY(6, 15); // farthest withdrawn
	printf("____  ");
	pspDebugScreenSetXY(6, 16);
	printf("  __\\____  ");
	pspDebugScreenSetXY(6, 17);
	printf("     ____)  ");
	pspDebugScreenSetXY(6, 18);
	printf("     _)  ");
	pspDebugScreenSetXY(6, 19);
	printf("    _)  ");
	pspDebugScreenSetXY(6, 20);
	printf("____)  ");
}
}
if (grey < 0x007FFFFE) {
if (grey > 0x00555554) {
	pspDebugScreenSetXY(6, 15); // halfway extended
	printf(" ____ ");
	pspDebugScreenSetXY(6, 16);
	printf("'  __\\____ ");
	pspDebugScreenSetXY(6, 17);
	printf("      ____) ");
	pspDebugScreenSetXY(6, 18);
	printf("      _) ");
	pspDebugScreenSetXY(6, 19);
	printf("     _) ");
	pspDebugScreenSetXY(6, 20);
	printf(".____) ");
}
}
if (grey < 0x00AAAAA8) {
if (grey > 0x007FFFFE) {
	pspDebugScreenSetXY(6, 15); // most extended
	printf("  ____");
	pspDebugScreenSetXY(6, 16);
	printf("-'  __\\____");
	pspDebugScreenSetXY(6, 17);
	printf("       ____)");
	pspDebugScreenSetXY(6, 18);
	printf("       _)");
	pspDebugScreenSetXY(6, 19);
	printf("      _)");
	pspDebugScreenSetXY(6, 20);
	printf("-.____)");
}
}
if (grey < 0x00D55552) {
if (grey > 0x00AAAAA8) {
	pspDebugScreenSetXY(6, 15); // halfway extended
	printf(" ____ ");
	pspDebugScreenSetXY(6, 16);
	printf("'  __\\____ ");
	pspDebugScreenSetXY(6, 17);
	printf("      ____) ");
	pspDebugScreenSetXY(6, 18);
	printf("      _) ");
	pspDebugScreenSetXY(6, 19);
	printf("     _) ");
	pspDebugScreenSetXY(6, 20);
	printf(".____) ");

}
}
if (grey > 0x00D55552) {
	pspDebugScreenSetXY(6, 15); // farthest withdrawn
	printf("____  ");
	pspDebugScreenSetXY(6, 16);
	printf("  __\\____  ");
	pspDebugScreenSetXY(6, 17);
	printf("     ____)  ");
	pspDebugScreenSetXY(6, 18);
	printf("     _)  ");
	pspDebugScreenSetXY(6, 19);
	printf("    _)  ");
	pspDebugScreenSetXY(6, 20);
	printf("____)  ");
}
}

void dither() {
		if (fetch == 17) {
		pspDebugScreenSetTextColor(colord); //white
		}
		if (fetch == 10) {
		pspDebugScreenSetTextColor(0x00000000); //black (invisible)
		}
		if (fetch == 24) {
		pspDebugScreenSetTextColor(0x00000000); //black (invisible)
		}
		if (fetch == 11) {
		pspDebugScreenSetTextColor(colorda); //
		}
		if (fetch == 23) {
		pspDebugScreenSetTextColor(colorda); //
		}
		if (fetch == 12) {
		pspDebugScreenSetTextColor(colordb); //
		}
		if (fetch == 22) {
		pspDebugScreenSetTextColor(colordb); //
		}
		if (fetch == 13) {
		pspDebugScreenSetTextColor(colordc); //
		}
		if (fetch == 21) {
		pspDebugScreenSetTextColor(colordc); //
		}
		if (fetch == 14) {
		pspDebugScreenSetTextColor(colordd); //
		}
		if (fetch == 20) {
		pspDebugScreenSetTextColor(colordd); //
		}
		if (fetch == 15) {
		pspDebugScreenSetTextColor(colorde); //
		}
		if (fetch == 19) {
		pspDebugScreenSetTextColor(colorde); //
		}
}

void scroll_dn() {
mlinet = mlineb;		// scroll menu down
mlineb = mlinec;		//
mlinec = mlined;		//
mlined = mlinee;		//
mlinee = mlinef;		//
mlinef = mlineg;		//
mlineg = mlinea;		//
mlinea = mlineo;		//
mlineo = mlineh;		//
mlineh = mlinei;		//
mlinei = mlinej;		//
mlinej = mlinek;		//
mlinek = mlinel;		//
mlinel = mlinem;		//
mlinem = mlinen;		//
mlinen = mlinet;		//

}

void scroll_up() {
mlinet = mlinen;		// scroll menu up
mlinen = mlinem;		//
mlinem = mlinel;		//
mlinel = mlinek;		//
mlinek = mlinej;		//
mlinej = mlinei;		//
mlinei = mlineh;		//
mlineh = mlineo;		//
mlineo = mlinea;		//
mlinea = mlineg;		//
mlineg = mlinef;		//
mlinef = mlinee;		//
mlinee = mlined;		//
mlined = mlinec;		//
mlinec = mlineb;		//
mlineb = mlinet;		//
}

void print_credits() {

		int yi;
		bounce_balls();
		if (fsshow == 0) {
				pspDebugScreenSetTextColor(color); // Blueish for header or footer
				pspDebugScreenSetXY(crlx, crla);
			        for(yi=0; yi<46; yi++) {
				printf("-");
				}

				pspDebugScreenSetTextColor(0x00EFDDCA);
				pspDebugScreenSetXY(crlx, crlb);
			        for(yi=0; yi<46; yi++) {
				crbuff = (credits[cr] ^ 0x3F);	//decrypt simple xor encrypted cedits
				translator();
				printf("%c",crbuff);
				cr++;
				}

				cr = cr - 45;

				if (cr > 2040) {
				cr = 0;
				}
				pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
				pspDebugScreenSetXY(crlx, crlc);
			        for(yi=0; yi<46; yi++) {
				printf("-");
				}
				}

				tripcol = tripcol + 0x00113FE2;
				if (tripcol > 0x00DF94CA) {
				tripcol = tripcol / menu;
				}

mcnt++;
if (mcnt > 200) {	// Credits Pong !

if (crld == 0) {
crla++;
crlb++;
crlc++;
if (crlc == 25) {
crld = 1;
}
}

if (crld == 1) {
crla--;
crlb--;
crlc--;
if (crla == 10) {
crld = 0;
}
}

if (crly == 0) {
crlx++;
if (crlx == 16) {
crly = 1;
}
}

if (crly == 1) {
crlx--;
if (crlx == 6) {
crly = 0;
}
}

if (crlx == 11) {
if (crla == 16) {
if (crlb == 17) {
if (crlc == 18) {
if (mcnt > 761) {
fsover = 0;		// free space game can be restarted
fsalive = 1;		// revive enemy
fshits = 0;		// reset number of hits taken
fsfire = 0;		// kill current bullet
amiga = 0;		// turn off super weapon
fsscore = 10800;	// reset score
fsx = 8;		// reset ship position
fsy = 17;		//
fbx = 8;		// reset bullet position
fby = 17;		//
mcnt = 0;
}
}
}
}
}
}
}

void print_creditsb() {
		int yb;
		bounce_balls();
		if (fsshow == 0) {
	pspDebugScreenSetTextColor(color); // Blueish for header or footer
	pspDebugScreenSetXY(crlx, crla);
			        for(yb=0; yb<46; yb++) {
				printf("-");
				}

	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(crlx, crlb);
			        for(yb=0; yb<46; yb++) {
				crbuff = (credits[cr] ^ 0x3F);	//decrypt simple xor encrypted cedits
				translator();
				printf("%c",crbuff);
				cr++;
				}
				cr = cr - 46;
				if (cr > 2040) {
				cr = 0;
				}
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	pspDebugScreenSetXY(crlx, crlc);
			        for(yb=0; yb<46; yb++) {
				printf("-");
				}
				}

}

void crfooter() {
//	pspDebugScreenSetXY(0, 25);
	pspDebugScreenSetTextColor(colorb); // header or footer
//	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';    Press Circle to return to X-Flash menu.   ;`-';");
	pspDebugScreenSetXY(0, 29);
if (easter == 1) {  // alert to easter egg triggered
	printf("      ;.-.;          1337 7r4n51a70r 4(7|va73d!          ;.-.;");
}
else {
	printf("      ;.-.;                                              ;.-.;");
}
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';              X-Flash by Art 2006.            ;`-';");
	pspDebugScreenSetTextColor(0x00EFDDCA);
}

void bounce_balls() {

if (mcnt > 200) {	// Check if credits bar is moving
if (mcnt < 471) {
print_starline();
play_freespace();
fs_message();
}
}
else {
clear_midscreen();
}

pspDebugScreenSetTextColor(tripcol);
pspDebugScreenSetXY(ballx, bally);

if (mcnt > 470) {
clear_midscreen();

if (mcnt < 550) { // bounce circles
bounce_circle();
bounce_tcircle();
}

if (mcnt > 525) { // draw line between the two moving points
if (mcnt < 600) {
xo = ballx;
yo = bally;
xi = tballx;
yi = tbally;
draw_line();
}
}

if (mcnt > 550) { // connect the sides to eack end of the line
if (mcnt < 600) {
xo = 61;
yo = 17;
xi = ballx;
yi = bally;
draw_line();
xo = 6;
yo = 17;
xi = tballx;
yi = tbally;
draw_line();
}
}

if (mcnt > 600) { // draw the pseudo 3D pyramid
if (mcnt < 650) {
xo = tballx;
yo = tbally;
xi = ballx + 6;
yi = bally + 6;
draw_line_slow();

xo = tballx;
yo = tbally;
xi = ballx - 6;
yi = bally + 6;
draw_line_slow();

xo = tballx;
yo = tbally;
xi = ballx - 6;
yi = bally - 6;
draw_line_slow();

xo = tballx;
yo = tbally;
xi = ballx + 6;
yi = bally - 6;
draw_line_slow();

xo = ballx + 6;
yo = bally;
xi = ballx + 6;
yi = bally + 6;
draw_line_slow();

xo = ballx;
yo = bally + 6;
xi = ballx - 6;
yi = bally + 6;
draw_line_slow();

xo = ballx - 6;
yo = bally;
xi = ballx - 6;
yi = bally - 6;
draw_line_slow();

xo = ballx;
yo = bally - 6;
xi = ballx + 6;
yi = bally - 6;
draw_line_slow();

xo = ballx;
yo = bally + 6;
xi = ballx + 6;
yi = bally + 6;
draw_line_slow();

xo = ballx - 6;
yo = bally;
xi = ballx - 6;
yi = bally + 6;
draw_line_slow();

xo = ballx;
yo = bally - 6;
xi = ballx - 6;
yi = bally - 6;
draw_line_slow();

xo = ballx + 6;
yo = bally;
xi = ballx + 6;
yi = bally - 6;
draw_line_slow();
}
}

if (mcnt > 650) {
if (mcnt < 731) {
draw_windows_circle();
}
}

if (mcnt > 730) {
if (mcnt < 761) {
draw_windows_circleb();
}
}

if (mcnt > 760) {
if (mcnt < 770) {
dcent = 34; // centre circle for start of circle tunnel demo
}
draw_demo_circle();
if (mcnt > 780) { // move circle tunnel centre

if (dircent == 0) {
dcent++;
if (dcent > 44) {
dircent = 1;
}
}
else {
dcent--;
if (dcent < 24) {
dircent = 0;
}
}

}
}
}

if (mcnt < 70) { //   adjustment to the time the eq will be on the display. must be set on the other line as well
if (mcnt < 38) {
printf("%c",smiley);
}
	mmb = mcnt - 8;
        int x, y, r2, tx, ty, xy;
        r2 = mmb * mmb;
        for (x = -mmb; x <= mmb; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = 34 + x;
	ty = 17 + y;
	xy = 17 - y;
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 24) {
	printf(".");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 24) {
	printf(".");
	}
	}
	}
	}

        }

}
	if (balla == 0) {
	ballx++;
	if (ballx > 61) {
	balla = 1;
	}
	}

	if (balla == 1) {
	ballx--;
	if (ballx < 6) {
	balla = 0;
	}
	}

if (mcnt < 200) {	// Check if credits bar is moving
if (mcnt > 500) {
if (bally == crlb) {
if (ballb == 0) {
ballb = 1;
}
else {
ballb = 0;
}

if (ballx < crlx) {
if (ballb == 0) {
ballb = 1;
}
else {
ballb = 0;
}
}

if (ballx > (crlx + 45)) {
if (ballb == 0) {
ballb = 1;
}
else {
ballb = 0;
}
}
}
}
}  // mcnt

	if (ballb == 0) {
	bally++;
	if (bally > 24) {
	ballb = 1;
	}
	}

	if (ballb == 1) {
	bally--;
	if (bally < 11) {
	ballb = 0;
	}
	}
	pspDebugScreenSetTextColor(tripcol);
	pspDebugScreenSetXY(tballx, tbally);

if (mcnt < 200) {
if (mcnt < 70) {
if (mcnt < 38) {
printf("%c",bsmiley);
}

if (mcnt < 35) {
draw_circle();
}
else {
circle_display();
}
	mmc++;
	if (mmc == 25) {
	mmc = 0;
	}
if (mcnt > 24) {
if (mcnt < 35) {
	mmc--;
	mmc--;
}
}

if (mcnt > 50) { //61
if (mcnt < 55) { //70
	mmc--;
	mmc--;
}
}
	}
	else {
	print_eq();
	}
	}

	if (tballa == 0) {
	tballx++;
	if (tballx > 61) {
	tballa = 1;
	}
	}

	if (tballa == 1) {
	tballx--;
	if (tballx < 6) {
	tballa = 0;
	}
	}

if (mcnt < 200) {	// Check if credits bar is moving
if (mcnt > 500) {	//
if (tbally == crlb) {
if (tballb == 0) {
tballb = 1;
}
else {
tballb = 0;
}

if (tballx < crlx) {
if (tballb == 0) {
tballb = 1;
}
else {
tballb = 0;
}
}

if (tballx > (crlx + 45)) {
if (tballb == 0) {
tballb = 1;
}
else {
tballb = 0;
}
}
}
}
} // mcnt

	if (tballb == 0) {
	tbally++;
	if (tbally > 24) {
	tballb = 1;
	}
	}

	if (tballb == 1) {
	tbally--;
	if (tbally < 11) {
	tballb = 0;
	}
	}
}

void clear_midscreen() {
	pspDebugScreenSetXY(0, 10);
	for(linecount=0; linecount<14; linecount++) {
	printf("                                                                   \n");
	}
}

void draw_chicken() {
	clear_midscreen();
	pspDebugScreenSetXY(0, 10);
	printf("                                          _\n");
	printf("                               .-.  .--''` )\n");
	printf("                            _ |  |/`   .-'`\n");
	printf("                           ( `\\      /`\n");
	printf("                           _)   _.  -'._ \n");
	printf("                         /`  .'     .-.-;\n");
	printf("                         `).'      /  \\  \\\n");
	printf("                        (`,        \\_o/_o/__ \n");
	printf("                         /           .-''`  ``'-.\n");
	printf("                         {         /` ,___.--''`\n");
	printf("                         {   ;     '-. \\ \\\n");
	printf("                         {   |'-....-`'.\\_\\\n");
	printf("                          \\   \\          ```\n");
	printf("                           \\   \\\n");
}

void print_footer() {
	pspDebugScreenSetXY(0, 25);
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetXY(0, 26);
	if (menu == 1) {
if (dzm == 0) {
	printf("      ;`-';          Remove or Restore the text:         ;`-';");
}
if (dzm == 1) {
	printf("      ;`-';                                              ;`-';");
}
	pspDebugScreenSetXY(0, 27);
if (dzm == 0) {
	printf("      ;.-.;         'Sony Computer Entertainment'        ;.-.;");
}
if (dzm == 1) {
	printf("      ;.-.;      Remove or Restore the PSP (tm) logo     ;.-.;");
}
	pspDebugScreenSetXY(0, 28);
if (dzm == 0) {
	printf("      ;`-';        from the cold boot opening screen     ;`-';");
}
if (dzm == 1) {
	printf("      ;`-';        from the gameboot opening screen      ;`-';");
}
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;   (has no effect on opening sound or waves). ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';    Press 'X' to remove, or 'O' to restore.   ;`-';");
	}

	if (menu == 2) {
	printf("      ;`-';   Change the standard opening sound effect,  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;   XMB menu error, or select sound effects.   ;.-.;");
	pspDebugScreenSetXY(0, 28);
	if (errs == 1) {
	printf("      ;`-'; Press 'X' to apply 'Game Show Buzzer' Sound  ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;  in place of the Original XMB error sound.   ;.-.;");
	}
	if (errs == 2) {
	printf("      ;`-'; Press 'X' to apply the Original error Sound  ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;     (Restores original XMB error sound).     ;.-.;");
	}
	if (errs == 3) {
	printf("      ;`-'; Press 'X' to apply External Sound with echo. ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;  Replaces opening sound with Coldboot.vag.   ;.-.;");
	}
	if (errs == 4) {
	printf("      ;`-'; Press 'X' to apply Air Wrench Opening Sound  ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;   in place of the Original Opening sound.    ;.-.;");
	}
	if (errs == 5) {
	printf("      ;`-';  Press 'X' to apply External Opening Sound   ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;  Replaces opening sound with Coldboot.vag.   ;.-.;");
	}
	if (errs == 6) {
	printf("      ;`-'; Press 'X' to delete the Opening Intro Sound. ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;          (Opening Intro is silent).          ;.-.;");
	}
	if (errs == 7) {
	printf("      ;`-';   Press 'X' to apply 'Heavy Click' Sound     ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.; in place of the Original XMB select sound.   ;.-.;");
	}
	if (errs == 8) {
	printf("      ;`-'; Press 'X' to apply the Original select Sound ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;    (Restores original XMB select sound).     ;.-.;");
	}
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-'; The 'O' button restores all original sounds. ;`-';");
	}

	if (menu == 3) {
	printf("      ;`-';   Press (X) to disable functionality of the  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;  Network Update icon that appears under the  ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';        Settings area of the XMB menu.        ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;   Firmware can still be updated from UMD if  ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';        version spoof is set correctly.       ;`-';");
	}

	if (menu == 4) {
	printf("      ;`-';      Press (X) to remove the wave effect     ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;          seen in the opening screen,         ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                and the XMB menu              ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;      Press (O) to restore the wave effect    ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
	}

	if (menu == 5) {

if (month == 1) {
print_optionf();
}
if (month == 2) {
print_optionf();
}
if (month == 3) {
print_optionf();
}
if (month == 4) {
print_optionf();
}
if (month == 5) {
print_optionf();
}
if (month == 6) {
print_optionf();
}
if (month == 7) {
print_optionf();
}
if (month == 8) {
print_optionf();
}
if (month == 9) {
print_optionf();
}
if (month == 10) {
print_optionf();
}
if (month == 11) {
print_optionf();
}
if (month == 12) {
print_optionf();
}
if (month == 13) {
	printf("      ;`-'; Replace the standard monthly menu background ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.; with a new background for all twelve months. ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;   Press the Circle button to restore the     ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';    original background for every month.      ;`-';");
}
if (month == 14) {
	printf("      ;`-'; Replace the standard monthly menu background ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.; with a plain black background for all months.;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;   Press the Circle button to restore the     ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';    original background for every month.      ;`-';");
}
if (month == 15) {
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;     Press the Circle button, or the Square   ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';   button to restore the original background  ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;               for every month.               ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
}
if (month == 16) {
	printf("      ;`-'; Apply a custom name plate picture that will  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.; appear in the PSP about screen that is found ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';    under System Settings in the XMB menu.    ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;  Press 'X' to apply the name plate picture,  ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';  Press 'O' to remove the name plate picture. ;`-';");
}
if (month == 17) {
	printf("      ;`-';    Mask the encryption information screens   ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;    found in System Settings/PSP About with   ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';              plain black pictures.           ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;  Press 'X' to apply the plain black screens, ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';  Press 'O' to restore the original screens.  ;`-';");
}
	}

	if (menu == 6) {
	printf("      ;`-';  Replace the standard XMB menu font with a   ;`-';");
	pspDebugScreenSetXY(0, 27);
	if (oea == 0) {
	printf("      ;.-.;                 custom font.                 ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                                              ;`-';");
	} // oea
	if (oea == 1) {
	printf("      ;.-.; custom font. Some custom fonts will prevent  ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-'; the Playstation emulator from working (3.02).;`-';");
	} // oea
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;      Restore the original Sony font with     ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';              the Circle button.              ;`-';");
	}

	if (menu == 7) {
if (game == 18) {
	printf("      ;`-';    Remove all trademark symbols from words   ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;      that appear in the XMB menu such as     ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';     UMD(tm), PSP(tm), and Memory Stick(tm).  ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;         Press 'X' to apply this change.      ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-'; Press 'O' to restore the trademark symbols.  ;`-';");
}
else {
if (game < 10) {
	printf("      ;`-';   Replaces the label of the Game icon that   ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;   appears in the XMB menu. The Game icon is  ;.-.;");
	pspDebugScreenSetXY(0, 28);
}
if (game > 9) {
if (game < 13) {
	printf("      ;`-';   Replaces the label of the Music icon that  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;   appears in the XMB menu. The Music icon is ;.-.;");
	pspDebugScreenSetXY(0, 28);
}
}
if (game > 12) {
if (game < 16) {
	printf("      ;`-';   Replaces the label of the Photo icon that  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;   appears in the XMB menu. The Photo icon is ;.-.;");
	pspDebugScreenSetXY(0, 28);
}
}
if (game > 15) {
if (game < 18) {
	printf("      ;`-';   Replaces the label of the Video icon that  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;   appears in the XMB menu. The Video icon is ;.-.;");
	pspDebugScreenSetXY(0, 28);
}
}
if (game == 19) {
	printf("      ;`-'; Press 'X' to open the Seven Hour Menu Editor.;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.; You can edit the XMB menu labels as desired, ;.-.;");
	pspDebugScreenSetXY(0, 28);
}
			if (game == 1) {
			printf("      ;`-';     renamed as 'Code' when (X) is pressed.   ;`-';");
			}
			if (game == 2) {
			printf("      ;`-';     renamed as 'Exec' when (X) is pressed.   ;`-';");
			}
			if (game == 3) {
			printf("      ;`-';     renamed as 'Game' when (X) is pressed.   ;`-';");
			}
			if (game == 4) {
			printf("      ;`-';     renamed as 'Gamz' when (X) is pressed.   ;`-';");
			}
			if (game == 5) {
			printf("      ;`-';     renamed as '1337' when (X) is pressed.   ;`-';");
			}
			if (game == 6) {
			printf("      ;`-';     renamed as 'Brew' when (X) is pressed.   ;`-';");
			}
			if (game == 7) {
			printf("      ;`-';     renamed as 'Apps' when (X) is pressed.   ;`-';");
			}
			if (game == 8) {
			printf("      ;`-';     renamed as '64MZ' when (X) is pressed.   ;`-';");
			}
			if (game == 9) {
			printf("      ;`-';     renamed as 'i505' when (X) is pressed.   ;`-';");
			}
			if (game == 10) {
			printf("      ;`-';     renamed as 'Tunes' when (X) is pressed.  ;`-';");
			}
			if (game == 11) {
			printf("      ;`-';     renamed as 'Music' when (X) is pressed.  ;`-';");
			}
			if (game == 12) {
			printf("      ;`-';     renamed as 'Audio' when (X) is pressed.  ;`-';");
			}
			if (game == 13) {
			printf("      ;`-';     renamed as 'Photo' when (X) is pressed.  ;`-';");
			}
			if (game == 14) {
			printf("      ;`-';     renamed as 'Porn' when (X) is pressed.   ;`-';");
			}
			if (game == 15) {
			printf("      ;`-';     renamed as 'Snaps' when (X) is pressed.  ;`-';");
			}
			if (game == 16) {
			printf("      ;`-';     renamed as 'Movie' when (X) is pressed.  ;`-';");
			}
			if (game == 17) {
			printf("      ;`-';     renamed as 'Video' when (X) is pressed.  ;`-';");
			}
			if (game == 19) {
			printf("      ;`-';  and save them for permanent use. This is a  ;`-';");
			}

	pspDebugScreenSetXY(0, 29);
if (game < 10) {
	printf("      ;.-.;   Press (O) to use the 'Game' label again.   ;.-.;");
}
if (game > 9) {
if (game < 13) {
	printf("      ;.-.;   Press (O) to use the 'Music' label again.  ;.-.;");
}
}
if (game > 12) {
if (game < 16) {
	printf("      ;.-.;   Press (O) to use the 'Photo' label again.  ;.-.;");
}
}
if (game > 15) {
if (game < 18) {
	printf("      ;.-.;   Press (O) to use the 'Video' label again.  ;.-.;");
}
}

if (game == 19) {
	printf("      ;.-.; rough clone of the Menuedit program by Loco, ;.-.;");
}
if (game != 19) {
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';    The actual directory name is unchanged.   ;`-';");
}
else {
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';    except editing is done on the PSP unit.   ;`-';");
}
	}
} // detm

	if (menu == 8) {
	printf("      ;`-';   Press 'X' to remove the Game Intro movie.  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;   Press 'O' to restore the Original movie.   ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-'; Press Square to apply the Plain Black movie. ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;  Press Triangle to apply your new gameboot,  ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';  checked for size. Be sure it is compatible! ;`-';");
	}

	if (menu == 9) {
if (oea == 0) {
	printf("      ;`-'; Applies a version spoof trick known to cause ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;      a 1.50 firmware version PSP to run      ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';    some games that it otherwise would not.   ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;     Any version 1.00 or above may be used.   ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';  Press Circle to remove this version spoof.  ;`-';");
} // oea
if (oea == 1) {
	printf("      ;`-';  Applies a version spoof trick that can be   ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;  seen in the PSP System Information screen.  ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';     Any version 1.00 or above may be used.   ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;     Press Circle to restore to 3.02 OE-B,    ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';    Square for OE-A, or Triangle for OE-C.    ;`-';");
} // oea
	}

	if (menu == 12) {
	printf("      ;`-'; Allows user to back up and restore Registry. ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;  This includes network settings, nickname,   ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-'; and other data that is entered by the user.  ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;  Press Cross to restore previous Registry.   ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';   Press Circle to perform registry backup.   ;`-';");
	}

if (menu == 11) {
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	if (rawdump == 0) {
	printf("      ;.-.;  Press Cross to perform a complete firmware  ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-'; dump. Firmware files are saved in the 'Dump' ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.; Directory found in the Xflash sub directory. ;.-.;");
	}
	if (rawdump == 1) {
	printf("      ;.-.;  Press Cross to perform a complete raw flash ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';  dump. Upper and Lower bin files are dumped  ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;     to the Xflash/Raw_Dump sub directory.    ;.-.;");
	}
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
}

	if (menu == 14) {
if (fms == 0) {
	printf("      ;`-';           Store or Retrieve an mp3           ;`-';");
}
if (fms == 1) {
	printf("      ;`-';         Delete a previously stored mp3       ;`-';");
}
if (fms == 2) {
	printf("      ;`-';      Apply custom firmware modification      ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;                 By Dark_AleX.                ;.-.;");
}
if (fms == 3) {
	printf("      ;`-';      Remove custom firmware modification     ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;                 By Dark_AleX.                ;.-.;");
}
if (fms == 4) {
	printf("      ;`-';   Open custom firmware configurator screen.  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;   This provides an easy way to set options   ;.-.;");
}
if (fms == 0) {
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;            file from flash memory.           ;.-.;");
}
if (fms == 1) {
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;            file from flash memory.           ;.-.;");
}
	pspDebugScreenSetXY(0, 28);
if (fms == 0) {
	printf("      ;`-'; Press 'X' to read MUSIC/Song.mp3 from flash0 ;`-';");
}
if (fms == 1) {
	printf("      ;`-';  Press 'X' to delete flash0:/MUSIC/Song.mp3  ;`-';");
}
if (fms == 2) {
	printf("      ;`-';   All of the flash operations are verified   ;`-';");
}
if (fms == 3) {
	printf("      ;`-';   All of the flash operations are verified   ;`-';");
}
if (fms == 4) {
	printf("      ;`-';      for the custom firmware modification.   ;`-';");
}
	pspDebugScreenSetXY(0, 29);
if (fms == 0) {
	printf("      ;.-.; Press 'O' to write MUSIC/Song.mp3 to flash0  ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';      Not tested with file over 3.6 Mb!!!     ;`-';");
}
if (fms == 1) {
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
}
if (fms == 2) {
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
}
if (fms == 3) {
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
}
if (fms == 4) {
	printf("      ;.-.;  This can also be done manually by editing   ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';  the file PSP/SYSTEM/config.txt. Press 'X'.  ;`-';");
}
if (fms == 5) {
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';            Press the Cross button            ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;      to open the PSP information screen.     ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;     This shows information read from the     ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';           PSP's internal NAND chip.          ;`-';");
}
		}

	if (menu == 15) {
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);

if (qpr == 0) {
	printf("      ;.-.;             Quits X-Flash program.           ;.-.;");
}
if (qpr == 1) {
	printf("      ;.-.;  Restores all original options that X-Flash  ;.-.;");
}
if (qpr == 2) {
	printf("      ;.-.;               Toggle test mode.              ;.-.;");
}
	pspDebugScreenSetXY(0, 28);
if (qpr == 0) {
	printf("      ;`-';    You can also exit with the HOME button.   ;`-';");
}
if (qpr == 1) {
	printf("      ;`-';   is able to perform, including Dark_AleX's  ;`-';");
}
if (qpr == 2) {
	printf("      ;`-';  Changes are made to the Xflash/Dump folder  ;`-';");
}
	pspDebugScreenSetXY(0, 29);
if (qpr == 0) {
	printf("      ;.-.;     Hold 'X' or hold 'O' to exit program,    ;.-.;");
}
if (qpr == 1) {
	printf("      ;.-.;       custom firmware if it is applied.      ;.-.;");
}
if (qpr == 2) {
	printf("      ;.-.;     instead of flash memory, for testing.    ;.-.;");
}

	pspDebugScreenSetXY(0, 30);
if (qpr == 1) {
	printf("      ;`-';  Hold 'X' to auto restore, and exit program. ;`-';");
}
if (qpr == 0) {
	printf("      ;`-';   keep holding to put PSP in standby mode.   ;`-';");
}
if (qpr == 2) {
	printf("      ;`-';     Press 'X' or 'O' to toggle test mode.    ;`-';");
}

if (qpr == 3) {
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;    Press 'X' to read the End User License    ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';     agreement, and program documentation.    ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
}

if (qpr == 4) {
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;     Press 'X' to start the credits demo.     ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';            Once started, you can             ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;   Press 'O' to return to the program menu.   ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
}

if (qpr == 5) {
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;         Press 'X' to play Free Space.        ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';            Once started, you can             ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;   Press 'O' to return to the program menu.   ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
}
	}

	if (menu == 13) {
	printf("      ;`-';  ");
		if (settings[0x64] == 0x31) {
		printf("                  CLASSIC   ");
		}
		if (settings[0x64] == 0x32) {
		printf("                  VIOLET    ");
		}
		if (settings[0x64] == 0x33) {
		printf("                 METALLIC   ");
		}
		if (settings[0x64] == 0x34) {
		printf("                  FUTURE    ");
		}
		if (settings[0x64] == 0x35) {
		printf("                  ORANGE    ");
		}
		if (settings[0x64] == 0x36) {
		printf("             AFRICAN  VIOLET");
		}
		if (settings[0x64] == 0x37) {
		printf("                   SKY      ");
		}
		if (settings[0x64] == 0x38) {
		printf("                   SEA      ");
		}
		if (settings[0x64] == 0x39) {
		printf("                 MILITARY   ");
		}
		if (settings[0x64] == 0x3A) {
		printf("                   AQUA     ");
		}
		if (settings[0x64] == 0x3B) {
		printf("                   FADED    ");
		}
		if (settings[0x64] == 0x3C) {
		printf("               A TAD 2 GH3Y ");
		}
		if (settings[0x64] == 0x3D) {
		printf("                MONOCHROME  ");
		}
		if (settings[0x64] == 0x3E) {
		printf("                  SUNSET    ");
		}
		if (settings[0x64] == 0x3F) {
		printf("             CHRISTMAS 2006!");
		}

	printf("                ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;     Change the X-Flash Menu colour theme,    ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';     or save it to be used for next time.     ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;     Press 'X' to save the current theme.     ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");

//		}
	}

	if (menu == 10) {
if (oea == 0) {
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;              Ph0r th3 R331 Phr34X            ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';         4 R3313 Phin3 Phirmw4r3 Phl45h       ;`-';");
} // oea
if (oea == 1) {
	printf("      ;`-';              Ph0r th3 R331 Phr34X            ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;         4 R3313 Phin3 Phirmw4r3 Phl45h       ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-'; Press Cross to apply the special 1337 spoof, ;`-';");
} // oea
	pspDebugScreenSetXY(0, 29);
if (oea == 0) {
	printf("      ;.-.;        83 4 1337 H4x0r! 'X'  0r n07 'O'      ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
} // oea
if (oea == 1) {
	printf("      ;.-.;     Press Circle to restore to 3.02 OE-B,    ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';     Square for OE-A, or Triangle for OE-C.   ;`-';");
} // oea
	}
	pspDebugScreenSetXY(0, 31);
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetTextColor(colord);
}

void print_optionf() {
	printf("      ;`-'; Replace all of the original menu backgrounds ;`-';");
	pspDebugScreenSetXY(0, 27);
if (oea == 0) {
	printf("      ;.-.; with a new background for the selected month.;.-.;");
}
if (oea == 1) {
	printf("      ;.-.;         with a new custom background.        ;.-.;");
}
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;   Press the Circle button to restore the     ;.-.;");
	pspDebugScreenSetXY(0, 30);
if (oea == 0) {
	printf("      ;`-'; original background for the selected month.  ;`-';");
}
if (oea == 1) {
	printf("      ;`-';    original backgrounds for every month.     ;`-';");
}
}

void print_message() {
if (sting == 0) {
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00FFFFFF); //white
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	print_header();
if (nomenutm == 0) {
	print_menu();
}
nomenutm = 0;

	int m = 0;
	pspDebugScreenSetXY(0, 25);
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;                                              ;.-.;");

	if (exist == 1) {
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                ");
	pspDebugScreenSetTextColor(colord);
	printf("FLASH COMPLETE");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("                ;`-';");
	}
	else {
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';         ");
	pspDebugScreenSetTextColor(colord);
	printf("    FILE DOES NOT EXIST    ");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("          ;`-';");
	}

	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 31);
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetTextColor(colord);

	for(m=0; m<36; m++) {
	sceDisplayWaitVblankStart();
	}
} // sting
else {
print_ua_footer();
}
	print_footer();
}

void print_regmessage() {
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00FFFFFF); //white
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	print_header();
	print_menu();
	print_blankfooter();

	finger_ok_animB();

	int q = 0;
	pspDebugScreenSetXY(0, 25);
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';            ");
	pspDebugScreenSetTextColor(colord);
	printf("REGISTRY BACKUP COMPLETE");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("          ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 31);
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetTextColor(colord);

	for(q=0; q<110; q++) {
	sceDisplayWaitVblankStart();
	}
	print_footer();
}

void print_regrestore() {
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00FFFFFF); //white
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	sceDisplayWaitVblankStart();
	pspDebugScreenInit();
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	print_header();
	print_menu();
	print_blankfooter();

	if (exist == 0) {
	finger_stop_anim();
	}
	else {
	finger_ok_animC();
	}

	int r = 0;
	pspDebugScreenSetXY(0, 25);
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetXY(0, 26);

	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;                                              ;.-.;");

	if (exist == 0) {
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';         ");
	pspDebugScreenSetTextColor(colord);
	printf("PERFORM REGISTRY BACKUP FIRST");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("        ;`-';");
	}
	else {
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';           ");
	pspDebugScreenSetTextColor(colord);
	printf("REGISTRY RESTORE COMPLETE");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("          ;`-';");
	}

	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 31);
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetTextColor(colord);

	for(r=0; r<120; r++) {
	sceDisplayWaitVblankStart();
	}
	print_footer();
}

void print_blankfooter() {
	print_header();
	print_justfooter();
	}

void print_justfooter() {
	pspDebugScreenSetXY(0, 25);
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 31);
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetTextColor(colord);
}

void print_header() {
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(color); // Blueish for header/footer
	pspDebugScreenSetXY(0, 1);
	printf("      ;");
	pspDebugScreenSetXY(0, 2);
	printf("      ;`-',");
	pspDebugScreenSetXY(0, 3);
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
	pspDebugScreenSetXY(0, 4);
	printf("      ;`-';    /|    /|                                  ;`-';");
	if (menu == 10) {
	pspDebugScreenSetXY(0, 5);
	printf("      ;.-.;   //|/''/''/                                 ;.-.;");
	}
	else {
	pspDebugScreenSetXY(0, 5);
	printf("      ;.-.;   //|/''/''/                                 ;.-.;");
	}
	if (menu == 10) {
	pspDebugScreenSetXY(0, 6);
	printf("      ;`-';  //''||' ||     ><-Ph145h %c%c%c%c   ",ver[9],ver[10],ver[11],ver[12]);
	printf("%c%c%c%c%c%c%c%c    ;`-';",ver[17],ver[18],ver[19],ver[20],ver[21],ver[22],ver[23],ver[24]);
	}
	else {
	pspDebugScreenSetXY(0, 6);
	printf("      ;`-';  //''||' ||      ><-Flash ");
	printf("%c%c%c%c%c%c%c%c%c%c",ver[9],ver[10],ver[11],ver[12],ver[13],ver[15],ver[16],ver[17],ver[18],ver[19]);
	printf("%c%c%c%c%c",ver[20],ver[21],ver[22],ver[23],ver[24]);
	printf("    ;`-';");
	}
	if (menu == 10) {
	pspDebugScreenSetXY(0, 7);
	printf("      ;.-.; |/   |/  |/                                  ;.-.;");
	}
	else {
	pspDebugScreenSetXY(0, 7);
	printf("      ;.-.; |/   |/  |/                                  ;.-.;");
	}
	pspDebugScreenSetXY(0, 8);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 9);
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");

	pspDebugScreenSetTextColor(0x00EEEEEE);
	pspDebugScreenSetXY(10, 1);
	if (test == 0) {
	printf("                                        ");
	}
	else {
	printf("              *** TEST MODE IS ON ***  ");
	}
	pspDebugScreenSetTextColor(color);
}

void increment_month() {
	month ++;
	if (month == 18) {
	month = 1;
	}
if (oea == 1) {	// skip individual month options for 3.02
if (month == 2) {
month = 14;
}
}
}

void decrement_month() {
	month --;
	if (month == 0) {
	month = 17;
	}
if (oea == 1) {	// skip individual month options for 3.02
if (month == 13) {
month = 1;
}
}
}

// read version.txt to RAM

void read_to_ram() {
	int fd;
		if (test == 0) {
		fd = sceIoOpen("flash0:/vsh/etc/version.txt", PSP_O_RDONLY, 0777);
		}
		else {
		fd = sceIoOpen("./Xflash/Dump/flash0/vsh/etc/version.txt", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fd,data,133);
	sceIoClose(fd);
}

// read version.txt to RAM

void read_dreg() {
	int sdrg;
		if (test == 0) {
		sdrg = sceIoOpen("flash1:/registry/system.dreg", PSP_O_RDONLY, 0777);
		}
		else {
		sdrg = sceIoOpen("./Xflash/Dump/flash1/registry/system.dreg", PSP_O_RDONLY, 0777);
		}
	sceIoRead(sdrg,dreg,9825);
	sceIoClose(sdrg);
}

void verify_index() {
	int fdb;
	int vfy;
	int cnt;
		if (test == 0) {
		fdb = sceIoOpen("flash0:/vsh/etc/index.dat", PSP_O_RDONLY, 0777);
		}
		else {
		fdb = sceIoOpen("./Xflash/Dump/flash0/vsh/etc/index.dat", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fdb,datab,133);
	sceIoClose(fdb);

	verify = 0;
	for(vfy=0; vfy<133; vfy++) {
		if (data[vfy] != datab[vfy]) {
		verify = 1;
		}
	}

	for(cnt=0; cnt<60; cnt++) {
	sceDisplayWaitVblankStart();
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
	for(ixt=0; ixt<170; ixt++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/etc/index.dat", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,data,133);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_index();
	}
	}
	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(cnt=0; cnt<60; cnt++) {
	sceDisplayWaitVblankStart();
	}
exist = 1;
}

void increment_dzm() {
dzm ++;
if (dzm == 2){
dzm = 0;
}
}

void decrement_dzm() {
dzm --;
if (dzm < 0){
dzm = 1;
}
}

void increment_qpr() {
qpr ++;
if (freespace == 1) {
if (qpr == 6){
qpr = 0;
}
}
else {
if (qpr == 5){
qpr = 0;
}
}
}

void decrement_qpr() {
qpr --;
if (freespace == 1) {
if (qpr < 0){
qpr = 5;
}
}
else {
if (qpr < 0){
qpr = 4;
}
}
}

void increment_fms() {
fms ++;
if (fms == 6){
fms = 0;
}
}

void decrement_fms() {
fms --;
if (fms < 0){
fms = 5;
}
}

void increment_vers() {

data[cpd] ++;
	if (data[cpd] == 58) {
	data [cpd] = 48;
	data [cpc] ++;
	}
if (data[cpc] == 58) {
data [cpc] = 48;
data [cpa] ++;
}
if (data[cpa] == 58) {
data [cpa] = 49;
}
}

void decrement_vers() {

data[cpd] --;
	if (data[cpd] == 47) {
	data [cpd] = 57;
	data [cpc] --;
	}
if (data[cpc] == 47) {
data [cpc] = 57;
data [cpa] --;
}
if (data[cpa] == 48) {
data [cpa] = 57;
}
}

void read_sysconf() {
	int fdx;
		if (test == 0) {
		fdx = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_RDONLY, 0777);
		}
		else {
		fdx = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fdx,syscon,sysconfsize);
	sceIoClose(fdx);

	chkk = 0;
	if (syscon[130732] != 49) {
	chkk = 1;
	}
}

void en1337() {
	int fdh;
	data[cpa] = 51; // set menu default version tricker value 3.37
	data[cpc] = 51;
	data[cpd] = 55;

	syscon[130732] = 49;//1 English
	syscon[130733] = 0;//
	syscon[130734] = 37;//%
	syscon[130735] = 0;//
	syscon[130736] = 49;//1

	syscon[143224] = 49;//1 French
	syscon[143225] = 0;//
	syscon[143226] = 37;//%
	syscon[143227] = 0;//
	syscon[143228] = 49;//1

	syscon[156380] = 49;//1 German
	syscon[156381] = 0;//
	syscon[156382] = 37;//%
	syscon[156383] = 0;//
	syscon[156384] = 49;//1

		if (test == 0) {
		fdh = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdh = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fdh,syscon,sysconfsize);
	sceIoClose(fdh);
}

void de1337() {
	int fdg;

	data[cpa] = 49; // set menu default version tricker value 1.50
	data[cpc] = 53;
	data[cpd] = 48;

	syscon[130732] = 37;//% English
	syscon[130733] = 0;//
	syscon[130734] = 49;//1
	syscon[130735] = 0;//
	syscon[130736] = 0;//

	syscon[143224] = 37;//% French
	syscon[143225] = 0;//
	syscon[143226] = 49;//1
	syscon[143227] = 0;//
	syscon[143228] = 0;//

	syscon[156380] = 37;//% German
	syscon[156381] = 0;//
	syscon[156382] = 49;//1
	syscon[156383] = 0;//
	syscon[156384] = 0;//

		if (test == 0) {
		fdg = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdg = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fdg,syscon,sysconfsize);
	sceIoClose(fdg);
}

void verify_sysconf() {
	int fxb;
	int vhy;
	int xcnt;
		if (test == 0) {
		fxb = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_RDONLY, 0777);
		}
		else {
		fxb = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fxb,vsys,sysconfsize);
	sceIoClose(fxb);

	verify = 0;
	for(vhy=0; vhy<sysconfsize; vhy++) {
		if (syscon[vhy] != vsys[vhy]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_blankfooter();
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,syscon,sysconfsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_sysconf();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(xcnt=0; xcnt<60; xcnt++) {
	sceDisplayWaitVblankStart();
	}
exist = 1;
}

void read_sysbg() {
	int xdx;
		if (test == 0) {
		xdx = sceIoOpen("flash0:/vsh/resource/system_plugin_bg.rco", PSP_O_RDONLY, 0777);
		}
		else {
		xdx = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin_bg.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(xdx,sysbg,systembgsize);
	sceIoClose(xdx);
}


void enable_wave() {
	if (seb == 0) {
	sysbg[1606] = 53;// Apply Wave Effect for 1.50
	}
	if (seb == 1) {
	sysbg[1606] = 79;// Apply Wave Effect for 2.71
	}
	if (oea == 1) {
	sysbg[1606] = 0x4F;// Apply Wave Effect for 3.02
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("       APPLY WAVE EFFECT ");

	for(xxnt=0; xxnt<110; xxnt++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		fxx = sceIoOpen("flash0:/vsh/resource/system_plugin_bg.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fxx = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin_bg.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fxx,sysbg,systembgsize);
	sceIoClose(fxx);
}

void disable_wave() {
	sysbg[1606] = 55;// Disable Wave Effect

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      DISABLE WAVE EFFECT ");

	for(xxnt=0; xxnt<110; xxnt++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		fxx = sceIoOpen("flash0:/vsh/resource/system_plugin_bg.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fxx = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin_bg.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fxx,sysbg,systembgsize);
	sceIoClose(fxx);
}

void verify_sysbg() {
	int fxmm;
	int vxs;
	int xcx;
		if (test == 0) {
		fxmm = sceIoOpen("flash0:/vsh/resource/system_plugin_bg.rco", PSP_O_RDONLY, 0777);
		}
		else {
		fxmm = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin_bg.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fxmm,vbg,systembgsize);
	sceIoClose(fxmm);

	verify = 0;
	for(vxs=0; vxs<systembgsize; vxs++) {
		if (sysbg[vxs] != vbg[vxs]) {
		verify = 1;
		}
	}


	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/resource/system_plugin_bg.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin_bg.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,sysbg,systembgsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_sysbg();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(xcx=0; xcx<60; xcx++) {
	sceDisplayWaitVblankStart();
	}
exist = 1;
}

// Read update_plugin.rco to RAM
void read_update() {
	int fd9;
		if (test == 0) {
		fd9 = sceIoOpen("flash0:/vsh/resource/update_plugin.rco", PSP_O_RDONLY, 0777);		// read update_plugin.rco to RAM
		}
		else {
		fd9 = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/update_plugin.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fd9,update,updatesize);
	sceIoClose(fd9);
}


void enable_update() {

if (seb == 0) {
	update[0] = 0x00;// Enable Network Update for 1.50
	update[1] = 0x50;//
	update[2] = 0x52;// 
	update[3] = 0x46;// 
	update[4] = 0x71;// 
	update[5] = 0x00;//
	update[6] = 0x00;// 
	update[7] = 0x00;// 
	update[8] = 0x00;// 
	update[9] = 0x00;//
 	update[10] = 0x00;//
	update[11] = 0x00;//
	update[12] = 0x00;//
	update[13] = 0x00;//
	update[14] = 0x00;//
	update[15] = 0x00;//
	update[16] = 0xA4;//
	update[17] = 0x00;//
	update[18] = 0x00;//
	update[19] = 0x00;//
	update[24] = 0x28;//
	update[25] = 0x01;//
	update[26] = 0x00;//
	update[27] = 0x00;//
	update[36] = 0xCC;//
	update[37] = 0x00;//
	update[38] = 0x00;//
	update[39] = 0x00;//
	update[48] = 0xE0;//
	update[49] = 0x13;//
 	update[50] = 0x00;//
	update[51] = 0x00;//
	update[56] = 0x80;//
	update[57] = 0x24;//
	update[58] = 0x00;//
	update[59] = 0x00;//
 	update[60] = 0x80;//
	update[61] = 0x8D;//
	update[62] = 0x00;//
	update[63] = 0x00;//
	update[64] = 0x00;//
	update[65] = 0xB2;//
	update[66] = 0x00;//
	update[67] = 0x00;//
	update[68] = 0xEC;//
	update[69] = 0x05;//
 	update[70] = 0x00;//
	update[71] = 0x00;//
	update[72] = 0xEC;//
	update[73] = 0xB7;//
	update[74] = 0x00;//
	update[75] = 0x00;//
	update[76] = 0x40;//
	update[77] = 0x01;//
	update[78] = 0x00;//
	update[79] = 0x00;//
 	update[80] = 0xB4;//
	update[81] = 0x1C;//
	update[82] = 0x00;//
	update[83] = 0x00;//
	update[84] = 0x80;//
	update[85] = 0x07;//
	update[86] = 0x00;//
	update[87] = 0x00;//
	update[88] = 0x34;//
	update[89] = 0x24;//
	update[90] = 0x00;//
	update[91] = 0x00;//
	update[92] = 0x08;//
	update[93] = 0x00;//
	update[94] = 0x00;//
	update[95] = 0x00;//
	update[100] = 0x00;// 
	update[101] = 0x00;// 
	update[102] = 0x00;// 
	update[103] = 0x00;// 
}
else {
	update[0] = 0x00;// Enable Network Update for 2.71
	update[1] = 0x50;//
	update[2] = 0x52;// 
	update[3] = 0x46;// 
	update[4] = 0x95;// 
	update[5] = 0x00;//
	update[6] = 0x00;// 
	update[7] = 0x00;// 
	update[8] = 0x00;// 
	update[9] = 0x00;//
 	update[10] = 0x00;//
	update[11] = 0x00;//
	update[12] = 0x20;//
	update[13] = 0x00;//
	update[14] = 0x00;//
	update[15] = 0x00;//
	update[16] = 0xA4;//
	update[17] = 0x00;//
	update[18] = 0x00;//
	update[19] = 0x00;//
	update[24] = 0x28;//
	update[25] = 0x01;//
	update[26] = 0x00;//
	update[27] = 0x00;//
	update[36] = 0xCC;//
	update[37] = 0x00;//
	update[38] = 0x00;//
	update[39] = 0x00;//
	update[48] = 0x90;//
	update[49] = 0x15;//
 	update[50] = 0x00;//
	update[51] = 0x00;//
	update[56] = 0x38;//
	update[57] = 0x0C;//
	update[58] = 0x00;//
	update[59] = 0x00;//
 	update[60] = 0x48;//
	update[61] = 0x22;//
	update[62] = 0x00;//
	update[63] = 0x00;//
	update[64] = 0xB0;//
	update[65] = 0x24;//
	update[66] = 0x00;//
	update[67] = 0x00;//
	update[68] = 0x40;//
	update[69] = 0x05;//
 	update[70] = 0x00;//
	update[71] = 0x00;//
	update[72] = 0xF0;//
	update[73] = 0x29;//
	update[74] = 0x00;//
	update[75] = 0x00;//
	update[76] = 0x40;//
	update[77] = 0x01;//
	update[78] = 0x00;//
	update[79] = 0x00;//
 	update[80] = 0x64;//
	update[81] = 0x1E;//
	update[82] = 0x00;//
	update[83] = 0x00;//
	update[84] = 0x00;//
	update[85] = 0x06;//
	update[86] = 0x00;//
	update[87] = 0x00;//
	update[88] = 0x64;//
	update[89] = 0x24;//
	update[90] = 0x00;//
	update[91] = 0x00;//
	update[92] = 0x08;//
	update[93] = 0x00;//
	update[94] = 0x00;//
	update[95] = 0x00;//
	update[100] = 0x00;// 
	update[101] = 0x00;// 
	update[102] = 0x00;// 
	update[103] = 0x00;// 
}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      ENABLE NETWORK UPDATE ");

	for(xxnu=0; xxnu<110; xxnu++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		nxx = sceIoOpen("flash0:/vsh/resource/update_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		nxx = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/update_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(nxx,update,updatesize);
	sceIoClose(nxx);
}

void disable_update() {
	update[0] = 0xFF;// Disable Network Update
	update[1] = 0xFF;//
	update[2] = 0xFF;// 
	update[3] = 0xFF;// 
	update[4] = 0xFF;// 
	update[5] = 0xFF;//
	update[6] = 0xFF;// 
	update[7] = 0xFF;// 
	update[8] = 0xFF;// 
	update[9] = 0xFF;// 
 	update[10] = 0xFF;//
	update[11] = 0xFF;//
	update[12] = 0xFF;//
	update[13] = 0xFF;//
	update[14] = 0xFF;//
	update[15] = 0xFF;//
	update[16] = 0xFF;//
	update[17] = 0xFF;//
	update[18] = 0xFF;//
	update[19] = 0xFF;//
	update[24] = 0xFF;//
	update[25] = 0xFF;//
	update[26] = 0xFF;//
	update[27] = 0xFF;//
	update[36] = 0xFF;//
	update[37] = 0xFF;//
	update[38] = 0xFF;//
	update[39] = 0xFF;//
	update[48] = 0xFF;//
	update[49] = 0xFF;//
 	update[50] = 0xFF;//
	update[51] = 0xFF;//
	update[56] = 0xFF;//
	update[57] = 0xFF;//
	update[58] = 0xFF;//
	update[59] = 0xFF;//
 	update[60] = 0xFF;//
	update[61] = 0xFF;//
	update[62] = 0xFF;//
	update[63] = 0xFF;//
	update[64] = 0xFF;//
	update[65] = 0xFF;//
	update[66] = 0xFF;//
	update[67] = 0xFF;//
	update[68] = 0xFF;//
	update[69] = 0xFF;//
 	update[70] = 0xFF;//
	update[71] = 0xFF;//
	update[72] = 0xFF;//
	update[73] = 0xFF;//
	update[74] = 0xFF;//
	update[75] = 0xFF;//
	update[76] = 0xFF;//
	update[77] = 0xFF;//
	update[78] = 0xFF;//
	update[79] = 0xFF;//
 	update[80] = 0xFF;//
	update[81] = 0xFF;//
	update[82] = 0xFF;//
	update[83] = 0xFF;//
	update[84] = 0xFF;//
	update[85] = 0xFF;//
	update[86] = 0xFF;//
	update[87] = 0xFF;//
	update[88] = 0xFF;//
	update[89] = 0xFF;//
	update[90] = 0xFF;//
	update[91] = 0xFF;//
	update[92] = 0xFF;//
	update[93] = 0xFF;//
	update[94] = 0xFF;//
	update[95] = 0xFF;//
	update[100] = 0xFF;// 
	update[101] = 0xFF;// 
	update[102] = 0xFF;// 
	update[103] = 0xFF;// 

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      DISABLE NETWORK UPDATE ");

	for(xxnu=0; xxnu<110; xxnu++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		nxx = sceIoOpen("flash0:/vsh/resource/update_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		nxx = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/update_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(nxx,update,updatesize);
	sceIoClose(nxx);
}

void verify_update() {
	int fxnu;
	int nus;
	int nux;
		if (test == 0) {
		fxnu = sceIoOpen("flash0:/vsh/resource/update_plugin.rco", PSP_O_RDONLY, 0777);
		}
		else {
		fxnu = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/update_plugin.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fxnu,vupd,updatesize);
	sceIoClose(fxnu);

	verify = 0;
	for(nus=0; nus<updatesize; nus++) {
		if (update[nus] != vupd[nus]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/resource/update_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/update_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,update,updatesize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_update();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(nux=0; nux<60; nux++) {
	sceDisplayWaitVblankStart();
	}
exist = 1;
}

//  Read topmenu_plugin.rco
void read_topmenu() {
	int xtm;
		if (test == 0) {
		xtm = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
		}
		else {
		xtm = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(xtm,opening,topmenusize);
	sceIoClose(xtm);
}

void txtenabled() {
	int xnue;

	opening[16904] = 0x4E;// 
	opening[16905] = 0x00;// 
	opening[16906] = 0x65;// 
	opening[16907] = 0x00;// 
	opening[16908] = 0x74;// 
	opening[16909] = 0x00;// 
	opening[16910] = 0x77;// 
	opening[16911] = 0x00;// 
	opening[16912] = 0x6F;// 
	opening[16913] = 0x00;// 
	opening[16914] = 0x72;// 
	opening[16915] = 0x00;// 
	opening[16916] = 0x6B;// 
	opening[16917] = 0x00;// 
	opening[16918] = 0x20;// 
	opening[16919] = 0x00;// 
	opening[16920] = 0x55;// 
	opening[16921] = 0x00;// 
	opening[16922] = 0x70;// 
	opening[16923] = 0x00;// 
	opening[16924] = 0x64;// 
	opening[16925] = 0x00;// 
	opening[16926] = 0x61;// 
	opening[16927] = 0x00;// 
	opening[16928] = 0x74;// 
	opening[16929] = 0x00;// 
	opening[16930] = 0x65;// 

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("   APPLY 'Network Update' LABEL ");

	for(xnue=0; xnue<110; xnue++) {
	sceDisplayWaitVblankStart();
	}

	int dhj;
	int dtm;
		if (test == 0) {
		dtm = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		dtm = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(dtm,opening,topmenusize);
	sceIoClose(dtm);

	for(dhj=0; dhj<100; dhj++) {
	sceDisplayWaitVblankStart();
	}
}

void txtdisabled() {
	int xnud;

	opening[16904] = 0x44;// 
	opening[16905] = 0x00;// 
	opening[16906] = 0x69;// 
	opening[16907] = 0x00;// 
	opening[16908] = 0x73;// 
	opening[16909] = 0x00;// 
	opening[16910] = 0x61;// 
	opening[16911] = 0x00;// 
	opening[16912] = 0x62;// 
	opening[16913] = 0x00;// 
	opening[16914] = 0x6C;// 
	opening[16915] = 0x00;// 
	opening[16916] = 0x65;// 
	opening[16917] = 0x00;// 
	opening[16918] = 0x64;// 
	opening[16919] = 0x00;// 
	opening[16920] = 0x20;// 
	opening[16921] = 0x00;// 
	opening[16922] = 0x20;// 
	opening[16923] = 0x00;// 
	opening[16924] = 0x20;// 
	opening[16925] = 0x00;// 
	opening[16926] = 0x20;// 
	opening[16927] = 0x00;// 
	opening[16928] = 0x20;// 
	opening[16929] = 0x00;//
	opening[16930] = 0x20;// 

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Disabled' LABEL ");

	for(xnud=0; xnud<110; xnud++) {
	sceDisplayWaitVblankStart();
	}

	int vhj;
	int vtm;
		if (test == 0) {
		vtm = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		vtm = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(vtm,opening,topmenusize);
	sceIoClose(vtm);

	for(vhj=0; vhj<100; vhj++) {
	sceDisplayWaitVblankStart();
	}
}

void verify_topmenu() {
	int uhj;
	int fxtv;
	int tma;
		if (test == 0) {
		fxtv = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
		}
		else {
		fxtv = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fxtv,vopening,topmenusize);
	sceIoClose(fxtv);

	verify = 0;
	for(tma=0; tma<topmenusize; tma++) {
		if (opening[tma] != vopening[tma]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,opening,topmenusize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_topmenu();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uhj=0; uhj<60; uhj++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void txtsave() {
	int xtue;

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);

	if (menu == 15) {
	if (qpr == 1) {
	printf("  RESTORE ORIGINAL MENU LABELS");
	}
	}
	else {
	printf("    APPLY CUSTOM MENU LABELS");
	}

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void txtgame() {
	int xnod;
	opening[16336] = 0x47;// G
	opening[16338] = 0x61;// a
	opening[16340] = 0x6D;// m
	opening[16342] = 0x65;// e

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY 'Game' LABEL ");

	for(xnod=0; xnod<110; xnod++) {
	sceDisplayWaitVblankStart();
	}

	int voj;
	int vom;

		if (test == 0) {
		vom = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		vom = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(vom,opening,topmenusize);
	sceIoClose(vom);

	for(voj=0; voj<100; voj++) {
	sceDisplayWaitVblankStart();
	}
}

void txtcode() {
	int xgod;
	opening[16336] = 0x43;// C
	opening[16338] = 0x6F;// o
	opening[16340] = 0x64;// d
	opening[16342] = 0x65;// e

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY 'Code' LABEL ");

	for(xgod=0; xgod<110; xgod++) {
	sceDisplayWaitVblankStart();
	}

	int vog;
	int gom;
		if (test == 0) {
		gom = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		gom = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(gom,opening,topmenusize);
	sceIoClose(gom);

	for(vog=0; vog<100; vog++) {
	sceDisplayWaitVblankStart();
	}
}

void txtexec() {
	int xexe;
	opening[16336] = 0x45;// E
	opening[16338] = 0x78;// x
	opening[16340] = 0x65;// e
	opening[16342] = 0x63;// c

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY 'Exec' LABEL ");

	for(xexe=0; xexe<110; xexe++) {
	sceDisplayWaitVblankStart();
	}

	int voa;
	int goa;
		if (test == 0) {
		goa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		goa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(goa,opening,topmenusize);
	sceIoClose(goa);

	for(voa=0; voa<100; voa++) {
	sceDisplayWaitVblankStart();
	}
}

void txttunes() {
	int xtue;
	opening[16312] = 0x54;// T
	opening[16314] = 0x75;// u
	opening[16316] = 0x6E;// n
	opening[16318] = 0x65;// e
	opening[16320] = 0x73;// s

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Tunes' LABEL ");

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void txtaudio() {
	int xtue;
	opening[16312] = 0x41;// A
	opening[16314] = 0x75;// u
	opening[16316] = 0x64;// d
	opening[16318] = 0x69;// i
	opening[16320] = 0x6F;// o

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Audio' LABEL ");

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void txtmusic() {
	int xtue;
	opening[16312] = 0x4D;// M
	opening[16314] = 0x75;// u
	opening[16316] = 0x73;// s
	opening[16318] = 0x69;// i
	opening[16320] = 0x63;// c

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Music' LABEL ");

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void txtphoto() {
	int xtue;
	opening[16300] = 0x50;// P
	opening[16302] = 0x68;// h
	opening[16304] = 0x6F;// o
	opening[16306] = 0x74;// t
	opening[16308] = 0x6F;// o

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Photo' LABEL ");

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void txtporn() {
	int xtue;
	opening[16300] = 0x50;// P
	opening[16302] = 0x6F;// o
	opening[16304] = 0x72;// r
	opening[16306] = 0x6E;// n
	opening[16308] = 0x00;// (0x20 , space)

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Porn' LABEL  ");

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void txtsnaps() {
	int xtue;
	opening[16300] = 0x53;// S
	opening[16302] = 0x6E;// n
	opening[16304] = 0x61;// a
	opening[16306] = 0x70;// p
	opening[16308] = 0x73;// s

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Snaps' LABEL ");

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void txtmovie() {
	int xtue;
	opening[16324] = 0x4D;// M
	opening[16326] = 0x6F;// o
	opening[16328] = 0x76;// v
	opening[16330] = 0x69;// i
	opening[16332] = 0x65;// e

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Movie' LABEL ");

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void txtvideo() {
	int xtue;
	opening[16324] = 0x56;// V
	opening[16326] = 0x69;// i
	opening[16328] = 0x64;// d
	opening[16330] = 0x65;// e
	opening[16332] = 0x6F;// o

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY 'Video' LABEL ");

	for(xtue=0; xtue<110; xtue++) {
	sceDisplayWaitVblankStart();
	}

	int toa;
	int uoa;
		if (test == 0) {
		uoa = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		uoa = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(uoa,opening,topmenusize);
	sceIoClose(uoa);

	for(toa=0; toa<100; toa++) {
	sceDisplayWaitVblankStart();
	}
}

void increment_game() {
	game ++;
	if (game == 20) {
	game = 1;
	}
}

void decrement_game() {
	game --;
	if (game == 0) {
	game = 19;
	}
}

void txtgamz() {
	int xgmz;
	opening[16336] = 0x47;// G
	opening[16338] = 0x61;// a
	opening[16340] = 0x6D;// m
	opening[16342] = 0x5A;// Z

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY 'GamZ' LABEL ");

	for(xgmz=0; xgmz<110; xgmz++) {
	sceDisplayWaitVblankStart();
	}

	int zgz;
	int ggz;
		if (test == 0) {
		ggz = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		ggz = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(ggz,opening,topmenusize);
	sceIoClose(ggz);

	for(zgz=0; zgz<100; zgz++) {
	sceDisplayWaitVblankStart();
	}
}

void txt1337() {
	int x337;
	opening[16336] = 0x31;// 1
	opening[16338] = 0x33;// 3
	opening[16340] = 0x33;// 3
	opening[16342] = 0x37;// 7

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY '1337' LABEL ");

	for(x337=0; x337<110; x337++) {
	sceDisplayWaitVblankStart();
	}

	int v37;
	int g37;
		if (test == 0) {
		g37 = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		g37 = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(g37,opening,topmenusize);
	sceIoClose(g37);

	for(v37=0; v37<100; v37++) {
	sceDisplayWaitVblankStart();
	}
}

void txtfilz() {
	int xflz;
	opening[16336] = 0x42;// B
	opening[16338] = 0x72;// r
	opening[16340] = 0x65;// e
	opening[16342] = 0x77;// w

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY 'Brew' LABEL ");

	for(xflz=0; xflz<110; xflz++) {
	sceDisplayWaitVblankStart();
	}

	int vfz;
	int gfz;
		if (test == 0) {
		gfz = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		gfz = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(gfz,opening,topmenusize);
	sceIoClose(gfz);

	for(vfz=0; vfz<100; vfz++) {
	sceDisplayWaitVblankStart();
	}
}

void txtapps() {
	int xaps;
	opening[16336] = 0x41;// A
	opening[16338] = 0x70;// p
	opening[16340] = 0x70;// p
	opening[16342] = 0x73;// s

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY 'Apps' LABEL ");

	for(xaps=0; xaps<110; xaps++) {
	sceDisplayWaitVblankStart();
	}

	int vap;
	int sap;
		if (test == 0) {
		sap = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		sap = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(sap,opening,topmenusize);
	sceIoClose(sap);

	for(vap=0; vap<100; vap++) {
	sceDisplayWaitVblankStart();
	}
}

void txt64mz() {
	int x64z;
	opening[16336] = 0x36;// 6
	opening[16338] = 0x34;// 4
	opening[16340] = 0x4D;// M
	opening[16342] = 0x5A;// Z

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY '64MZ' LABEL ");

	for(x64z=0; x64z<110; x64z++) {
	sceDisplayWaitVblankStart();
	}

	int v64;
	int s64;
		if (test == 0) {
		s64 = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		s64 = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(s64,opening,topmenusize);
	sceIoClose(s64);

	for(v64=0; v64<100; v64++) {
	sceDisplayWaitVblankStart();
	}
}

void txti505() {
	int x505;
	opening[16336] = 0x69;// i
	opening[16338] = 0x35;// 5
	opening[16340] = 0x30;// 0
	opening[16342] = 0x35;// 5

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY 'i505' LABEL ");

	for(x505=0; x505<110; x505++) {
	sceDisplayWaitVblankStart();
	}

	int v05;
	int s05;
		if (test == 0) {
		s05 = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		s05 = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(s05,opening,topmenusize);
	sceIoClose(s05);

	for(v05=0; v05<100; v05++) {
	sceDisplayWaitVblankStart();
	}
}

void read_system() {
	int xsp;

	if (test == 0) {
	xsp = sceIoOpen("flash0:/vsh/resource/system_plugin.rco", PSP_O_RDONLY, 0777);
	}
	else {
	xsp = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin.rco", PSP_O_RDONLY, 0777);
	}

	sceIoRead(xsp,systp,systemsize);
	sceIoClose(xsp);

}

void verify_system() {
	int fxvs;
	int vsa;
	int xsv;

	if (test == 0) {
	fxvs = sceIoOpen("flash0:/vsh/resource/system_plugin.rco", PSP_O_RDONLY, 0777);
	}
	else {
	fxvs = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin.rco", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvs,systv,systemsize);
	sceIoClose(fxvs);

	verify = 0;
	for(vsa=0; vsa<systemsize; vsa++) {
		if (systp[vsa] != systv[vsa]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,systp,systemsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_system();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(xsv=0; xsv<60; xsv++) {
	sceDisplayWaitVblankStart();
	}
exist = 1;
}

void mod_click() {

if (seb == 0) {
	vmi = 0;
	for(vmd=64516; vmd<65620; vmd++) {
		systp[vmd] = se5click[vmi];
		vmi ++;
	}
}

if (seb == 1) {
	vmi = 0;
	for(vmd=53604; vmd<54692; vmd++) {
		systp[vmd] = se5click[vmi];
		vmi ++;
	}

}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    APPLY NEW XMB CLICK SOUND ");

	for(esnt=0; esnt<110; esnt++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		fes = sceIoOpen("flash0:/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fes = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fes,systp,systemsize);
	sceIoClose(fes);
}

void org_click() {

if (seb == 0) {
	qmi = 0;
	for(qmd=64516; qmd<65620; qmd++) {
		systp[qmd] = se5org[qmi];
		qmi ++;
	}
} // seb

if (seb == 1) {
	qmi = 0;
	for(qmd=53604; qmd<54692; qmd++) {
		systp[qmd] = okvag[qmi];
		qmi ++;
	}
} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY ORIGINAL XMB CLICK SOUND ");

	for(esno=0; esno<110; esno++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	feo = sceIoOpen("flash0:/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	feo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(feo,systp,systemsize);
	sceIoClose(feo);
}

void mod_error() {

if (seb == 0) {
	vmi = 0;
	for(vmd=74532; vmd<94788; vmd++) {
		systp[vmd] = errorm[vmi];
		vmi ++;
	}
} // seb

if (seb == 1) {
	vmi = 0;
	for(vmd=63620; vmd<83876; vmd++) {
		systp[vmd] = errorm[vmi];
		vmi ++;
	}
} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    APPLY NEW XMB ERROR SOUND ");

	for(esnt=0; esnt<110; esnt++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		fes = sceIoOpen("flash0:/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fes = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fes,systp,systemsize);
	sceIoClose(fes);
}

void org_error() {

if (seb == 0) {
	qmi = 0;
	for(qmd=74532; qmd<94788; qmd++) {
		systp[qmd] = erroro[qmi];
		qmi ++;
	}
} // seb

if (seb == 1) {
	qmi = 0;
	for(qmd=63620; qmd<83876; qmd++) {
		systp[qmd] = erroro[qmi];
		qmi ++;
	}
} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY ORIGINAL XMB ERROR SOUND ");

	for(esno=0; esno<110; esno++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	feo = sceIoOpen("flash0:/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	feo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/system_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(feo,systp,systemsize);
	sceIoClose(feo);
}

void increment_error() {
	errs ++;
	if (errs == 9) {
	errs = 1;
	}
}

void decrement_error() {
	errs --;
	if (errs == 0) {
	errs = 8;
	}
}

void read_ltn0() {
	int ltx;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/ltn0.pgf", PSP_O_RDONLY, 0777);
	font_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	ltx = sceIoOpen("./Xflash/Modified/ltn0.pgf", PSP_O_RDONLY, 0777);
	sceIoRead(ltx,ltn0,10);
	sceIoClose(ltx);

	disq = 0;

	if (font_file_size > 1000000) {  //Check size of pgf file
	disq = 1;
	}

	if (ltn0[4] != 0x50) {  //Check for 'P' of "PGF file"
	disq = 1;
	}
}

void read_settings() {
	int stg;

	stg = sceIoOpen("./Xflash/Xflash/Theme.XF", PSP_O_RDONLY, 0777);
	sceIoRead(stg,settings,0xB5);
	sceIoClose(stg);
}

void apply_settings() {

	if (settings[0x64] == 0x31){ //Check for Old X-Flash
	color = 0x00339900;
	colorb = 0x00330099;
	colorc = 0x00444444;
	colord = 0x00FFFFFF;
	}
	if (settings[0x64] == 0x32){ //Check for Violet
	color = 0x00330099;
	colorb = 0x00330099;
	colorc = 0x00333333;
	colord = 0x00FFFFFF;
	}
	if (settings[0x64] == 0x33){ //Check for Metallic
	color = 2109782;
	colorb = 2109782;
	colorc = 0x00333333;
	colord = 0x00EFEFEF;
	}
	if (settings[0x64] == 0x34){ //Check for Future
	color = 0x006F6F6F;
	colorb = 0x006F6F6F;
	colorc = 0x00333333;
	colord = 0x00008D8F; //yellow?
	}
	if (settings[0x64] == 0x35){ //Check for Orange
	color = 285887217;
	colorb = 285887217;
	colorc = 0x00333333;
	colord = 0x00FCFCFC;
	}
	if (settings[0x64] == 0x36){ //Check for Violet / Yellow
	color = 0x00330099;
	colorb = 0x00330099;
	colorc = 0x00333333;
	colord = 0x00008D8F; //yellow?
	}
	if (settings[0x64] == 0x37){ //Check for Blue / Yellow
	color = 0x00EE0C0C;
	colorb = 0x00EE0C0C;
	colorc = 0x00333333;
	colord = 0x0022BDBF; //yellow?
	}
	if (settings[0x64] == 0x38){ //Check for Blue
	color = 0x00FF4A4A;
	colorb = 0x00FF4A4A;
	colorc = 0x00333333;
	colord = 0x00FFDFDF;
	}
	if (settings[0x64] == 0x39){ //Check for Military
	color = 0x00509373;
	colorb = 0x00376028;
	colorc = 0x00333333;
	colord = 0x00E8E0E5;
	}
	if (settings[0x64] == 0x3A){ //Check for Aqua
	color = 0x008F6F12; // aqua
	colorb = 0x008F6F12; // aqua
	colorc = 0x00333333; // grey
	colord = 0x00E8E0E5; // white
	}
	if (settings[0x64] == 0x3B){ //Check for Faded
	color = 0x004372AA;
	colorb = 0x006E6E6E;
	colorc = 0x00333333;
	colord = 0x00C8C0C5; // white
	}
	if (settings[0x64] == 0x3C){ //Check for A Tad Gh3y
	color = 0x00BA64FE; // 
	colorb = 0x00BA64FE; //
	colorc = 0x00333333; //
	colord = 0x00FF00CC; //
	}
	if (settings[0x64] == 0x3D){ //Check for Monochrome
	color = 0x00E8E0E5; // white
	colorb = 0x00E8E0E5; // white
	colorc = 0x00333333; // grey
	colord = 0x00E8E0E5; // white
	}
	if (settings[0x64] == 0x3E){ //Check for Sunset Red
	color = 0x001111FE;
	colorb = 0x001111FE;
	colorc = 0x00333333;
	colord = 0x001010FE;
	}
	if (settings[0x64] == 0x3F){ //Check for Christmas
	color = 0x000303EF;
	colorb = 0x0003EF03;
	colorc = 0x00333333;
	colord = 0x00C0C0C0; // white
	}
	if (test == 1) {
	color = 0x00008D8F; //test mode theme (chicken mode)
	colorb = 0x00008D8F;
	colorc = 0x00333333;
	colord = 0x00008D8F;
	}

colorda = colorc - 0x00282828; //calculate dither colours for menu screen
colordb = colorc - 0x00202020;
colordc = colorc - 0x00181818;
colordd = colorc - 0x00101010;
colorde = colorc - 0x00080808;
}

void save_settings() {
	int sstg;

	sstg = sceIoOpen("./Xflash/Xflash/Theme.XF", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	sceIoWrite(sstg,settings,0xB5);
	sceIoClose(sstg);
}

void read_bmp() {
	int bmp;
	bmp = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(bmp,back,25);
	sceIoClose(bmp);

	bmp = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	file_size = sceIoLseek32(bmp, 0, SEEK_END);
	sceIoClose(bmp);

	disb = 0;

	if (back[0] != 0x42) {  //Check for 'B' of "BMP file"
	disb = 1;
	}

	if (file_size > 391734) {  // Check if background.bmp is bigger than 382 KB (391,734 bytes) 480x272.
	disb = 1;
	}


resx = back[19] * 0x100;
resx = resx + back[18];
resy = back[23] * 0x100;
resy = resy + back[22];
}

void read_png() {
	int bmp;
	bmp = sceIoOpen("./xflash/Modified/name_plate.png", PSP_O_RDONLY, 0777);
	sceIoRead(bmp,back,25);
	sceIoClose(bmp);

	bmp = sceIoOpen("./xflash/Modified/name_plate.png", PSP_O_RDONLY, 0777);
	file_size = sceIoLseek32(bmp, 0, SEEK_END);
	sceIoClose(bmp);

	disb = 0;

	if (back[1] != 0x50) {  //Check for 'P' of "PNG file"
	disb = 1;
	}

	if (file_size > 391734) {  // Check if name_plate.png is bigger than 382 KB (391,734 bytes) 480x272.
	disb = 1;
	}


resx = back[18] * 0x100;
resx = resx + back[19];
resy = back[22] * 0x100;
resy = resy + back[23];
}

void gameboottoram() {
	int gbtr;
	int gbk;
	int qmo;

	for(qmo=0; qmo<200704; qmo++) {
		gbram[qmo] = gameboot[qmo];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" APPLY ORIGINAL GAME BOOT MOVIE");

	for(gbtr=0; gbtr<110; gbtr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	gbk = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	gbk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(gbk,gbram,200704);
	sceIoClose(gbk);
}

void blackgameboottoram() {
	int gbtr;
	int gbk;
	int qmo;


if (seb == 0) {
	for(qmo=0; qmo<331776; qmo++) {
	bgbram[qmo] = blackboot[qmo];
	}
} // seb

if (seb == 1) {
	for(qmo=0; qmo<10240; qmo++) {
	bgbram[qmo] = seblack[qmo];
	}
} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY BLACK GAME BOOT MOVIE  ");

	for(gbtr=0; gbtr<110; gbtr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	gbk = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	gbk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

if (seb == 0) {
	sceIoWrite(gbk,bgbram,331776);
	sceIoClose(gbk);
} // seb

if (seb == 1) {
	sceIoWrite(gbk,bgbram,10240);
	sceIoClose(gbk);
} // seb

}

void fonttoram() {
	int fttr;
	int ftk;
	int qmy;

	if (seb == 0) {
	for(qmy=0; qmy<123896; qmy++) {
		fntram[qmy] = font[qmy];
	}
	} // seb

	if (seb == 1) {
	for(qmy=0; qmy<69108; qmy++) {
		fntram[qmy] = sefont[qmy];
	}
	} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY ORIGINAL XMB FONT ");

	for(fttr=0; fttr<110; fttr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	ftk = sceIoOpen("flash0:/font/ltn0.pgf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	ftk = sceIoOpen("./Xflash/Dump/flash0/font/ltn0.pgf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	if (seb == 0) {
	sceIoWrite(ftk,fntram,123896);
	} // seb
	if (seb == 1) {
	sceIoWrite(ftk,fntram,69108);
	} // seb
	sceIoClose(ftk);
}

void verify_gameboot() {
	int ugb;
	int fxgb;
	int tpl;

	if (test == 0) {
	fxgb = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_RDONLY, 0777);
	}
	else {
	fxgb = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxgb,gbram,200704);
	sceIoClose(fxgb);

	verify = 0;
	for(tpl=0; tpl<200704; tpl++) {
		if (gbram[tpl] != gameboot[tpl]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,gameboot,200704);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_gameboot();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(ugb=0; ugb<60; ugb++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_blackgameboot() {
	int ugb;
	int fxgb;
	int tpl;

	if (test == 0) {
	fxgb = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_RDONLY, 0777);
	}
	else {
	fxgb = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_RDONLY, 0777);
	}

if (seb == 0) {
	sceIoRead(fxgb,bgbram,331776);
	sceIoClose(fxgb);
} // seb

if (seb == 1) {
	sceIoRead(fxgb,bgbram,10240);
	sceIoClose(fxgb);
} // seb

	verify = 0;

if (seb == 0) {
	for(tpl=0; tpl<331776; tpl++) {
		if (bgbram[tpl] != blackboot[tpl]) {
		verify = 1;
		}
	}
} // seb

if (seb == 1) {
	for(tpl=0; tpl<10240; tpl++) {
		if (bgbram[tpl] != seblack[tpl]) {
		verify = 1;
		}
	}
} // seb

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	if (seb == 0) {
		sceIoWrite(fdo,blackboot,331776);
		sceIoClose(fdo);
	} // seb

	if (seb == 1) {
		sceIoWrite(fdo,seblack,10240);
		sceIoClose(fdo);
	} // seb

		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_gameboot();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(ugb=0; ugb<60; ugb++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_font() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/font/ltn0.pgf", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/font/ltn0.pgf", PSP_O_RDONLY, 0777);
	}
	if (seb == 0) {
	sceIoRead(fxvf,vfntram,123896);
	} // seb
	if (seb == 1) {
	sceIoRead(fxvf,vfntram,69108);
	}
	sceIoClose(fxvf);


	if (seb == 0) {
	verify = 0;
	for(vpp=0; vpp<123896; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}
	} // seb

	if (seb == 1) {
	verify = 0;
	for(vpp=0; vpp<69108; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}
	} // seb

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/font/ltn0.pgf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/font/ltn0.pgf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	if (seb == 0) {
		sceIoWrite(fdo,fntram,123896);
	} // seb
	if (seb == 1) {
		sceIoWrite(fdo,fntram,69108);
	} // seb
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_font();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_user_font() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/font/ltn0.pgf", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/font/ltn0.pgf", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,font_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<font_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/font/ltn0.pgf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/font/ltn0.pgf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,font_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_user_font();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_font_file() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/ltn0.pgf", PSP_O_RDONLY, 0777);
	font_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	ltff = sceIoOpen("./Xflash/Modified/ltn0.pgf", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,font_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY CUSTOM XMB FONT ");

	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/font/ltn0.pgf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/font/ltn0.pgf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,font_file_size);
	sceIoClose(fte);

}

// read opening_plugin.rco to RAM

void read_opening() {
	int optr;

	if (test == 0) {
	optr = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_RDONLY, 0777);
	}
	else {
	optr = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_RDONLY, 0777);
	}

	sceIoRead(optr,opening,openingsize);
	sceIoClose(optr);
}

void verify_opening() {
	int opy;
	int fopx;
	int opl;

	if (test == 0) {
	fopx = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_RDONLY, 0777);
	}
	else {
	fopx = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fopx,vopening,openingsize);
	sceIoClose(fopx);

	verify = 0;
	for(opl=0; opl<openingsize; opl++) {
		if (opening[opl] != vopening[opl]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,opening,openingsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_opening();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(opy=0; opy<60; opy++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void dewaveop() {
	int dwop;

	if (seb == 0) {
	opening[236772] = 0x00; // remove wave from opening for 1.50
	opening[236773] = 0x00; // 
	opening[236774] = 0x00; // 
	opening[236775] = 0x00; // 
	opening[236776] = 0x00; // 
	opening[236777] = 0x00; // 
	opening[236778] = 0x00; // 
	opening[236779] = 0x00; //
	} // seb
	if (seb == 1) {
	opening[217584] = 0x55; // remove wave from opening for 2.71 SE
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     REMOVE OPENING WAVE ");

	for(dwop=0; dwop<110; dwop++) {
	sceDisplayWaitVblankStart();
	}

	int ote;
	int ogw;
		if (test == 0) {
		ogw = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		ogw = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(ogw,opening,openingsize);
	sceIoClose(ogw);

	for(ote=0; ote<100; ote++) {
	sceDisplayWaitVblankStart();
	}
}

void enwaveop() {
	int enop;

	if (seb == 0) {
	opening[236772] = 0x49; // restore wave in opening for 1.50
	opening[236773] = 0x05; // 
	opening[236774] = 0x15; // 
	opening[236775] = 0x24; // 
	opening[236776] = 0x19; // 
	opening[236777] = 0x00; // 
	opening[236778] = 0x23; // 
	opening[236779] = 0x8A; // 
	} // seb
	if (seb == 1) {
	opening[217584] = 0xC7; // restore wave in opening for 2.71 SE
	}
	if (oea == 1) {
	opening[217584] = 0xD7; // restore wave in opening for 3.02 OE
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY OPENING WAVE ");

	for(enop=0; enop<110; enop++) {
	sceDisplayWaitVblankStart();
	}

	int otd;
	int ogd;
		if (test == 0) {
		ogd = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		ogd = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(ogd,opening,openingsize);
	sceIoClose(ogd);

	for(otd=0; otd<100; otd++) {
	sceDisplayWaitVblankStart();
	}
}

void enbrandop() {
	int dwap;

if (seb == 0) {
	opening[32796] = 0xE2;// Restore branding for 1.50
}
if (seb == 1) {
	opening[12716] = 0x67;// Restore branding for 2.71 SE
}
if (oea == 1) {
	opening[12716] = 0x9A;// Restore branding for 3.02 OE
}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    RESTORE INTRO BRANDING ");

	for(dwap=0; dwap<110; dwap++) {
	sceDisplayWaitVblankStart();
	}

	int oae;
	int agw;
		if (test == 0) {
		agw = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		agw = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(agw,opening,openingsize);
	sceIoClose(agw);

	for(oae=0; oae<100; oae++) {
	sceDisplayWaitVblankStart();
	}
}

void debrandop() {
	int ebrp;

if (seb == 0) {
	opening[32796] = 0x02;// Remove branding for 1.50
}
if (seb == 1) {
	opening[12716] = 0x55;// Remove branding for 2.71 SE
}
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    REMOVE INTRO BRANDING ");

	for(ebrp=0; ebrp<110; ebrp++) {
	sceDisplayWaitVblankStart();
	}

	int obr;
	int orb;
		if (test == 0) {
		orb = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		orb = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(orb,opening,openingsize);
	sceIoClose(orb);

	for(obr=0; obr<100; obr++) {
	sceDisplayWaitVblankStart();
	}
}

void enbrandgb() {
	int gwap;

if (seb == 0) {
	opening[34992] = 0x8A;// Restore branding 1.50
}
if (seb == 1) {
	opening[14868] = 0x56;// Restore branding 2.71 SE
}
if (oea == 1) {
	opening[14868] = 0x7D;// Restore branding 3.02 OE
}
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     RESTORE GAMEBOOT LOGO ");
	for(gwap=0; gwap<110; gwap++) {
	sceDisplayWaitVblankStart();
	}

	int gae;
	int ggw;
		if (test == 0) {
		ggw = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		ggw = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(ggw,opening,openingsize);
	sceIoClose(ggw);

	for(gae=0; gae<100; gae++) {
	sceDisplayWaitVblankStart();
	}
}

void debrandgb() {
	int gbrp;

if (seb == 0) {
	opening[34992] = 0x00;// Remove branding for 1.50
}
if (seb == 1) {
	opening[14868] = 0x55;// Remove branding for 2.71 SE
}
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     REMOVE GAMEBOOT LOGO ");
	for(gbrp=0; gbrp<110; gbrp++) {
	sceDisplayWaitVblankStart();
	}

	int gbr;
	int grb;
		if (test == 0) {
		grb = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		grb = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(grb,opening,openingsize);
	sceIoClose(grb);

	for(gbr=0; gbr<100; gbr++) {
	sceDisplayWaitVblankStart();
	}
}

void delete_opening_vag() {
	int vag;
	int vagt;

	if (seb == 0) {	// delete opening vag for 1.50
	for(vag=35064; vag<135864; vag++) {
	opening[vag] = 0;
	}
	for(vag=135944; vag<236744; vag++) {
	opening[vag] = 0;
	}
	} // seb

	if (seb == 1) {	// delete opening vag for 2.71
	if (oea == 0) {	
	for(vag=15220; vag<116020; vag++) {
	opening[vag] = 0;
	}
	for(vag=116100; vag<216900; vag++) {
	opening[vag] = 0;
	}
	} // oea
	} // seb

	if (seb == 1) {	// delete opening vag for 3.02
	if (oea == 1) {	
	for(vag= 0x3B98; vag<0x1C558; vag++) {
	opening[vag] = 0;
	}
	for(vag= 0x1C5A8; vag<0x34F68; vag++) {
	opening[vag] = 0;
	}
	} // oea
	} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  CREATE BLANK VAG FILE IN RAM ");

	for(vagt=0; vagt<110; vagt++) {
	sceDisplayWaitVblankStart();
	}
}

void actual_delete() {
	int vas;
		if (test == 0) {
		vas = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		vas = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(vas,opening,openingsize);
	sceIoClose(vas);
}

void original_opening() {
	int eoro;
	int oro;
	int qro;
	int qrh;

if (seb == 0) {
	qrh = 0x40;
	for(qro=0x88F8; qro<0x212B8; qro++) {
	if (qrh < 0x18A00) {
		opening[qro] = openinga[qrh];	//check for end of shorter file
		qrh ++;
	}
	}
	qrh = 0x40;
	for(qro=0x21308; qro<0x39CC8; qro++) {
	if (qrh < 0x18A00) {
		opening[qro] = openingb[qrh];	//check for end of shorter file
		qrh ++;
	}
	}
} // seb

if (seb == 1) {
if (oea == 0) {
	qrh = 0x40;
	for(qro=0x3B74; qro<0x1C534; qro++) {
	if (qrh < 0x18A00) {
		opening[qro] = openinga[qrh];	//check for end of shorter file
		qrh ++;
	}
	}
	qrh = 0x40;
	for(qro=0x1C5A4; qro<0x34F44; qro++) {
	if (qrh < 0x18A00) {
		opening[qro] = openingb[qrh];	//check for end of shorter file
		qrh ++;
	}
	}
} // oea
} // seb

if (seb == 1) {
if (oea == 1) {
	qrh = 0x40;
	for(qro=0x3B98; qro<0x1C558; qro++) {
	if (qrh < 0x18A00) {
		opening[qro] = openinga[qrh];	//check for end of shorter file
		qrh ++;
	}
	}
	qrh = 0x40;
	for(qro=0x1C5A8; qro<0x34F68; qro++) {
	if (qrh < 0x18A00) {
		opening[qro] = openingb[qrh];	//check for end of shorter file
		qrh ++;
	}
	}
} // oea
} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY ORIGINAL OPENING SOUND  ");

	for(eoro=0; eoro<110; eoro++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	oro = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	oro = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(oro,opening,openingsize);
	sceIoClose(oro);
}

void wrench_opening() {
	int eowo;
	int owo;
	int swo;
	int srw;

if (seb == 0) {
	srw = 0x40;
	for(swo=0x88F8; swo<0x212B8; swo++) {
	if (srw < 0x5260) {
		opening[swo] = wrench[srw];	//check for end of shorter file
		srw ++;
	}
	}
	srw = 0x40;
	for(swo=0x21308; swo<0x39CC8; swo++) {
	if (srw < 0x5260) {
		opening[swo] = wrench[srw];	//check for end of shorter file
		srw ++;
	}
	}
} // seb

if (seb == 1) {
if (oea == 0) {
	srw = 0x40;
	for(swo=0x3B74; swo<0x1C534; swo++) {
	if (srw < 0x5260) {
		opening[swo] = wrench[srw];	//check for end of shorter file
		srw ++;
	}
	}
	srw = 0x40;
	for(swo=0x1C584; swo<0x34F44; swo++) {
	if (srw < 0x5260) {
		opening[swo] = wrench[srw];	//check for end of shorter file
		srw ++;
	}
	}
} // oea
} // seb

if (seb == 1) {
if (oea == 1) {
	srw = 0x40;
	for(swo=0x3B98; swo<0x1C558; swo++) {
	if (srw < 0x5260) {
		opening[swo] = wrench[srw];	//check for end of shorter file
		srw ++;
	}
	}
	srw = 0x40;
	for(swo=0x1C5A8; swo<0x34F68; swo++) {
	if (srw < 0x5260) {
		opening[swo] = wrench[srw];	//check for end of shorter file
		srw ++;
	}
	}
} // oea
} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" APPLY AIR WRENCH OPENING SOUND  ");

	for(eowo=0; eowo<110; eowo++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	owo = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	owo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(owo,opening,openingsize);
	sceIoClose(owo);
}

void coldboot_opening() {

	int opxz;

	for(opxz=0; opxz<100880; opxz++) {
	coldboot[opxz] = 0;
	}

	opxz = sceIoOpen("./xflash/Modified/Coldboot.vag", PSP_O_RDONLY, 0777);
	sceIoRead(opxz,coldboot,100880);
	sceIoClose(opxz);

	int eiro;
	int aro;
	int uro;
	int urh;

if (seb == 0) {
	urh = 0x40;
	for(aro=0x88F8; aro<0x212B8; aro++) {
	if (urh < 0x18A00) {
		opening[aro] = coldboot[urh];	//check for end of shorter file
		urh ++;
	}
	}
	urh = 0x40;
	for(aro=0x21308; aro<0x39CC8; aro++) {
	if (urh < 0x18A00) {
		opening[aro] = coldboot[urh];	//check for end of shorter file
		urh ++;
	}
	}
} // seb

if (seb == 1) {
if (oea == 0) {
	urh = 0x40;
	for(aro=0x3B74; aro<0x1C534; aro++) {
	if (urh < 0x18A00) {
		opening[aro] = coldboot[urh];	//check for end of shorter file
		urh ++;
	}
	}
	urh = 0x40;
	for(aro=0x1C584; aro<0x34F44; aro++) {
	if (urh < 0x18A00) {
		opening[aro] = coldboot[urh];	//check for end of shorter file
		urh ++;
	}
	}
} // oea
} // seb

if (seb == 1) {
if (oea == 1) {
	urh = 0x40;
	for(aro=0x3B98; aro<0x1C558; aro++) {
	if (urh < 0x18A00) {
		opening[aro] = coldboot[urh];	//check for end of shorter file
		urh ++;
	}
	}
	urh = 0x40;
	for(aro=0x1C5A8; aro<0x34F68; aro++) {
	if (urh < 0x18A00) {
		opening[aro] = coldboot[urh];	//check for end of shorter file
		urh ++;
	}
	}
} // oea
} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY EXTERNAL OPENING SOUND  ");

	for(eiro=0; eiro<110; eiro++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	uro = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	uro = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(uro,opening,openingsize);
	sceIoClose(uro);
}

void coldboot_echo() {

	int opxe;

	for(opxe=0; opxe<100880; opxe++) {
		coldboot[opxe] = 0;
	}

	opxe = sceIoOpen("./xflash/Modified/Coldboot.vag", PSP_O_RDONLY, 0777);
	sceIoRead(opxe,coldboot,100880);
	sceIoClose(opxe);

	int eoee;
	int oee;
	int eeo;
	int eoh;

if (seb == 0) {
	eoh = 0x40;
	for(eeo=0x88F8; eeo<0x212B8; eeo++) {
	if (eoh < 100700) {
		opening[eeo] = coldboot[eoh];	//check for end of shorter file
		eoh ++;
	}
	}
	eoh = 0x40;
	for(eeo=0x23A08; eeo<0x39CC8; eeo++) {  //this value will adjust echo delay
//	for(eeo=0x21308; eeo<0x39CC8; eeo++) {
	if (eoh < 100700) {
		opening[eeo] = coldboot[eoh];	//check for end of shorter file
		eoh ++;
	}
	}
} // seb

if (seb == 1) {
if (oea == 0) {
	eoh = 0x40;
	for(eeo=0x3B74; eeo<0x1C534; eeo++) {
	if (eoh < 100700) {
		opening[eeo] = coldboot[eoh];	//check for end of shorter file
		eoh ++;
	}
	}
	eoh = 0x40;
	for(eeo=0x1EC84; eeo<0x34F44; eeo++) {  //this value will adjust echo delay
//	for(eeo=0x1C584; eeo<0x34F44; eeo++) {
	if (eoh < 100700) {
		opening[eeo] = coldboot[eoh];	//check for end of shorter file
		eoh ++;
	}
	}
} // oea
} // seb

if (seb == 1) {
if (oea == 1) {
	eoh = 0x40;
	for(eeo=0x3B98; eeo<0x1C558; eeo++) {
	if (eoh < 100700) {
		opening[eeo] = coldboot[eoh];	//check for end of shorter file
		eoh ++;
	}
	}
	eoh = 0x40;
	for(eeo=0x1ECA8; eeo<0x34F68; eeo++) {  //this value will adjust echo delay
//	for(eeo=0x1C5A8; eeo<0x34F68; eeo++) {
	if (eoh < 100700) {
		opening[eeo] = coldboot[eoh];	//check for end of shorter file
		eoh ++;
	}
	}
} // oea
} // seb

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" APPLY EXTERNAL SOUND WITH ECHO");

	for(eoee=0; eoee<110; eoee++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	oee = sceIoOpen("flash0:/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	oee = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/opening_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(oee,opening,openingsize);
	sceIoClose(oee);
}

void read_coldboot() {
	int rcb;

	rcb = sceIoOpen("./xflash/Modified/Coldboot.vag", PSP_O_RDONLY, 0777);
	sceIoRead(rcb,coldboot,100880);
	sceIoClose(rcb);
}

void alltoram() {
	int janr;
	int jak;
	int jay;
	int cnt;

	cnt = 0;

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = jan[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = feb[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = mar[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = apr[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = may[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = jun[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = jul[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = aug[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = sep[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = oct[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = nov[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = dec[jay];
	cnt++;
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" APPLY EVERY ORIGINAL BACKGROUND ");
	for(janr=0; janr<110; janr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	jak = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	jak = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(jak,allpics,74112);
	sceIoClose(jak);
}

void jantoram() {
	int janr;
	int jay;

	for(jay=0; jay<6176; jay++) {
	janram[jay] = jan[jay];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("APPLY ORIGINAL JANUARY BACKGROUND");
	for(janr=0; janr<110; janr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	jak = sceIoOpen("flash0:/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	jak = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(jak,janram,6176);
	sceIoClose(jak);
}

void febtoram() {
	int febr;
	int febk;
	int feby;

	for(feby=0; feby<6176; feby++) {
	janram[feby] = feb[feby];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("APPLY ORIGINAL FEBRUARY BACKGROUND");
	for(febr=0; febr<110; febr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	febk = sceIoOpen("flash0:/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	febk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(febk,janram,6176);
	sceIoClose(febk);
}

void martoram() {
	int marr;
	int mark;
	int mary;

	for(mary=0; mary<6176; mary++) {
	janram[mary] = mar[mary];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" APPLY ORIGINAL MARCH BACKGROUND");
	for(marr=0; marr<110; marr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	mark = sceIoOpen("flash0:/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	mark = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(mark,janram,6176);
	sceIoClose(mark);
}

void aprtoram() {
	int aprr;
	int aprk;
	int apry;

	for(apry=0; apry<6176; apry++) {
	janram[apry] = apr[apry];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" APPLY ORIGINAL APRIL BACKGROUND");
	for(aprr=0; aprr<110; aprr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	aprk = sceIoOpen("flash0:/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	aprk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(aprk,janram,6176);
	sceIoClose(aprk);
}

void maytoram() {
	int mayr;
	int mayk;
	int mayy;

	for(mayy=0; mayy<6176; mayy++) {
	janram[mayy] = may[mayy];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY ORIGINAL MAY BACKGROUND");
	for(mayr=0; mayr<110; mayr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	mayk = sceIoOpen("flash0:/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	mayk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(mayk,janram,6176);
	sceIoClose(mayk);
}

void juntoram() {
	int junr;
	int junk;
	int juny;

	for(juny=0; juny<6176; juny++) {
	janram[juny] = jun[juny];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY ORIGINAL JUNE BACKGROUND");
	for(junr=0; junr<110; junr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	junk = sceIoOpen("flash0:/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	junk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(junk,janram,6176);
	sceIoClose(junk);
}

void jultoram() {
	int julr;
	int julk;
	int july;

	for(july=0; july<6176; july++) {
	janram[july] = jul[july];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" APPLY ORIGINAL JULY BACKGROUND");
	for(julr=0; julr<110; julr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	julk = sceIoOpen("flash0:/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	julk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(julk,janram,6176);
	sceIoClose(julk);
}

void augtoram() {
	int augr;
	int augk;
	int augy;

	for(augy=0; augy<6176; augy++) {
	janram[augy] = aug[augy];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("APPLY ORIGINAL AUGUST BACKGROUND");
	for(augr=0; augr<110; augr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	augk = sceIoOpen("flash0:/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	augk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(augk,janram,6176);
	sceIoClose(augk);
}

void septoram() {
	int sepr;
	int sepk;
	int sepy;

	for(sepy=0; sepy<6176; sepy++) {
	janram[sepy] = sep[sepy];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("APPLY ORIGINAL SEPTEMBER BACKGROUND");
	for(sepr=0; sepr<110; sepr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	sepk = sceIoOpen("flash0:/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	sepk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(sepk,janram,6176);
	sceIoClose(sepk);
}

void octtoram() {
	int octr;
	int octk;
	int octy;

	for(octy=0; octy<6176; octy++) {
	janram[octy] = oct[octy];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("APPLY ORIGINAL OCTOBER BACKGROUND");
	for(octr=0; octr<110; octr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	octk = sceIoOpen("flash0:/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	octk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(octk,janram,6176);
	sceIoClose(octk);
}

void novtoram() {
	int novr;
	int novk;
	int novy;

	for(novy=0; novy<6176; novy++) {
	janram[novy] = nov[novy];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("APPLY ORIGINAL NOVEMBER BACKGROUND");
	for(novr=0; novr<110; novr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	novk = sceIoOpen("flash0:/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	novk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(novk,janram,6176);
	sceIoClose(novk);
}

void dectoram() {
	int decr;
	int deck;
	int decy;

	for(decy=0; decy<6176; decy++) {
	janram[decy] = dec[decy];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("APPLY ORIGINAL DECEMBER BACKGROUND");
	for(decr=0; decr<110; decr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	deck = sceIoOpen("flash0:/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	deck = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(deck,janram,6176);
	sceIoClose(deck);
}

void blktoram() {

	int blkr;
	int blky;

if (seb == 1) {
if (oea == 0) {
	vbmpsize = 58;
}
}
if (seb == 1) {
if (oea == 1) {
	vbmpsize = 6176;
}
}
if (seb == 0) {
	vbmpsize = 6176;
}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("CREATE PLAIN BLACK BITMAP FILE");
	for(blkr=0; blkr<100; blkr++) {
	sceDisplayWaitVblankStart();
	}

if (seb == 0) {

	for(blky=0; blky<vbmpsize; blky++) {
	janram[blky] = 0;
	}

	janram[0] = 0x42; // create black background bitmap file 1.50
	janram[1] = 0x4D;
	janram[2] = 0x1E;
	janram[3] = 0x18;
	janram[10] = 0x36;
	janram[14] = 0x28;
	janram[18] = 0x3C;
	janram[22] = 0x22;
	janram[26] = 0x01;
	janram[28] = 0x18;
} // seb

if (seb == 1) {
if (oea == 0) {

	for(blky=0; blky<vbmpsize; blky++) {
	janram[blky] = 0;
	}

	janram[0] = 0x42; // create black background bitmap file 2.71 SE
	janram[1] = 0x4D;
	janram[2] = 0x3A;
	janram[10] = 0x36;
	janram[14] = 0x28;
	janram[18] = 0x01;
	janram[22] = 0x01;
	janram[26] = 0x01;
	janram[28] = 0x18;
} // oea
} // seb

if (seb == 1) {
if (oea == 1) {

	for(blky=0; blky<vbmpsize; blky++) {
	janram[blky] = 0;
	}

	janram[0] = 0x42; // create black background bitmap file 3.02
	janram[1] = 0x4D;
	janram[2] = 0x1E;
	janram[3] = 0x18;
	janram[10] = 0x36;
	janram[14] = 0x28;
	janram[18] = 0x3C;
	janram[22] = 0x22;
	janram[26] = 0x01;
	janram[28] = 0x18;
} // oea
} // seb

if (oea == 1) {
	int cnt;
	cnt = 0;
	int jay;

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = janram[jay];
	cnt++;
	}
}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
if (oea == 0) {
	printf("  APPLY PLAIN BLACK BACKGROUND 1   ");
}
else {
	printf("  APPLY PLAIN BLACK BACKGROUNDS    ");
}
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}



if (oea == 0) {
	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
}
if (oea == 1) {
	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
}




if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
if (oea == 0) {
	sceIoWrite(blkk,janram,58);
} // oea
} // seb
if (seb == 1) {
if (oea == 1) {
	sceIoWrite(blkk,allpics,74112);
} // oea
} // seb
	sceIoClose(blkk);
	verify_jan();


if (oea == 1) {
return;
}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 2");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_feb();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 3");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_mar();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 4");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_apr();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 5");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_may();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 6");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_jun();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 7");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_jul();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 8");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_aug();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 9");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_sep();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 10");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_oct();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 11");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_nov();

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY PLAIN BLACK BACKGROUND 12");
	for(blkr=0; blkr<110; blkr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	blkk = sceIoOpen("flash0:/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	blkk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}


if (seb == 0) {
	sceIoWrite(blkk,janram,6176);
} // seb
if (seb == 1) {
	sceIoWrite(blkk,janram,58);
} // seb
	sceIoClose(blkk);
	verify_dec();
	
	vbmpsize = 6176;
}

void verify_all() {
	int janvf;
	int fxjan;
	int janpp;

	if (test == 0) {
	fxjan = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxjan = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxjan,vjanram,74112);
	sceIoClose(fxjan);

	verify = 0;
	for(janpp=0; janpp<74112; janpp++) {
		if (allpics[janpp] != vjanram[janpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,74112);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_all();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(janvf=0; janvf<60; janvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_jan() {
	int janvf;
	int janpp;

if (oea == 0) {
	if (test == 0) {
	fxjan = sceIoOpen("flash0:/vsh/resource/01.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxjan = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01.bmp", PSP_O_RDONLY, 0777);
	}
} // oea
if (oea == 1) {
	if (test == 0) {
	fxjan = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxjan = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_RDONLY, 0777);
	}
} // oea


if (oea == 0) {
	sceIoRead(fxjan,vjanram,vbmpsize);
	sceIoClose(fxjan);
}
if (oea == 1) {
	sceIoRead(fxjan,vjanram,74112);
	sceIoClose(fxjan);
}

	verify = 0;

if (oea == 0) {
	for(janpp=0; janpp<vbmpsize; janpp++) {
		if (janram[janpp] != vjanram[janpp]) {
		verify = 1;
		}
	}
} // oea
if (oea == 1) {
	for(janpp=0; janpp<74112; janpp++) {
		if (allpics[janpp] != vjanram[janpp]) {
		verify = 1;
		}
	}
} // oea

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

if (oea == 0) {
	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
} // oea
if (oea == 1) {
	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
} // oea
		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_jan();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(janvf=0; janvf<60; janvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_feb() {
	int febvf;
	int fxfeb;
	int febpp;

	if (test == 0) {
	fxfeb = sceIoOpen("flash0:/vsh/resource/02.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxfeb = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/02.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxfeb,vjanram,vbmpsize);
	sceIoClose(fxfeb);

	verify = 0;
	for(febpp=0; febpp<vbmpsize; febpp++) {
		if (janram[febpp] != vjanram[febpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_feb();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(febvf=0; febvf<60; febvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_mar() {
	int marvf;
	int fxmar;
	int marpp;

	if (test == 0) {
	fxmar = sceIoOpen("flash0:/vsh/resource/03.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxmar = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/03.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxmar,vjanram,vbmpsize);
	sceIoClose(fxmar);

	verify = 0;
	for(marpp=0; marpp<vbmpsize; marpp++) {
		if (janram[marpp] != vjanram[marpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_mar();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(marvf=0; marvf<60; marvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_apr() {
	int aprvf;
	int fxapr;
	int aprpp;

	if (test == 0) {
	fxapr = sceIoOpen("flash0:/vsh/resource/04.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxapr = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/04.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxapr,vjanram,vbmpsize);
	sceIoClose(fxapr);

	verify = 0;
	for(aprpp=0; aprpp<vbmpsize; aprpp++) {
		if (janram[aprpp] != vjanram[aprpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_apr();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(aprvf=0; aprvf<60; aprvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_may() {
	int mayvf;
	int fxmay;
	int maypp;

	if (test == 0) {
	fxmay = sceIoOpen("flash0:/vsh/resource/05.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxmay = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/05.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxmay,vjanram,vbmpsize);
	sceIoClose(fxmay);

	verify = 0;
	for(maypp=0; maypp<vbmpsize; maypp++) {
		if (janram[maypp] != vjanram[maypp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_may();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(mayvf=0; mayvf<60; mayvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_jun() {
	int junvf;
	int fxjun;
	int junpp;

	if (test == 0) {
	fxjun = sceIoOpen("flash0:/vsh/resource/06.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxjun = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/06.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxjun,vjanram,vbmpsize);
	sceIoClose(fxjun);

	verify = 0;
	for(junpp=0; junpp<vbmpsize; junpp++) {
		if (janram[junpp] != vjanram[junpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_jun();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(junvf=0; junvf<60; junvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_jul() {
	int julvf;
	int fxjul;
	int julpp;

	if (test == 0) {
	fxjul = sceIoOpen("flash0:/vsh/resource/07.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxjul = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/07.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxjul,vjanram,vbmpsize);
	sceIoClose(fxjul);

	verify = 0;
	for(julpp=0; julpp<vbmpsize; julpp++) {
		if (janram[julpp] != vjanram[julpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_jul();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(julvf=0; julvf<60; julvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_aug() {
	int augvf;
	int fxaug;
	int augpp;

	if (test == 0) {
	fxaug = sceIoOpen("flash0:/vsh/resource/08.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxaug = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/08.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxaug,vjanram,vbmpsize);
	sceIoClose(fxaug);

	verify = 0;
	for(augpp=0; augpp<vbmpsize; augpp++) {
		if (janram[augpp] != vjanram[augpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_aug();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(augvf=0; augvf<60; augvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_sep() {
	int sepvf;
	int fxsep;
	int seppp;

	if (test == 0) {
	fxsep = sceIoOpen("flash0:/vsh/resource/09.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxsep = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/09.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxsep,vjanram,vbmpsize);
	sceIoClose(fxsep);

	verify = 0;
	for(seppp=0; seppp<vbmpsize; seppp++) {
		if (janram[seppp] != vjanram[seppp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_sep();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(sepvf=0; sepvf<60; sepvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_oct() {
	int octvf;
	int fxoct;
	int octpp;

	if (test == 0) {
	fxoct = sceIoOpen("flash0:/vsh/resource/10.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxoct = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/10.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxoct,vjanram,vbmpsize);
	sceIoClose(fxoct);

	verify = 0;
	for(octpp=0; octpp<vbmpsize; octpp++) {
		if (janram[octpp] != vjanram[octpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_oct();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(octvf=0; octvf<60; octvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_nov() {
	int novvf;
	int fxnov;
	int novpp;

	if (test == 0) {
	fxnov = sceIoOpen("flash0:/vsh/resource/11.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxnov = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/11.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxnov,vjanram,vbmpsize);
	sceIoClose(fxnov);

	verify = 0;
	for(novpp=0; novpp<vbmpsize; novpp++) {
		if (janram[novpp] != vjanram[novpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_nov();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(novvf=0; novvf<60; novvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void verify_dec() {
	int decvf;
	int fxdec;
	int decpp;

	if (test == 0) {
	fxdec = sceIoOpen("flash0:/vsh/resource/12.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxdec = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/12.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxdec,vjanram,vbmpsize);
	sceIoClose(fxdec);

	verify = 0;
	for(decpp=0; decpp<vbmpsize; decpp++) {
		if (janram[decpp] != vjanram[decpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,janram,vbmpsize);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_dec();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(decvf=0; decvf<60; decvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void blank_pmf() {
	int bpmfr;
	int bpmfk;
	int bpmfy;

	for(bpmfy=0; bpmfy<18; bpmfy++) {
	blankpmf[bpmfy] = 0;
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  APPLY BLANK GAME BOOT MOVIE");
	for(bpmfr=0; bpmfr<100; bpmfr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	bpmfk = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	bpmfk = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(bpmfk,blankpmf,0);
	sceIoClose(bpmfk);
	verify_blankpmf();
}

void verify_blankpmf() {
	int pmfvf;
	int fdv;

	if (test == 0) {
	fdv = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_RDONLY, 0);
	}
	else {
	fdv = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_RDONLY, 0);
	}

	if(fdv < 0) {
		exist = 0;
        }
	else {
                exist = 1;
        }
	sceIoClose(fdv);

	verify = 0;
	if (exist == 0) {
	verify = 1;
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,blankpmf,0);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_blankpmf();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(pmfvf=0; pmfvf<60; pmfvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_gameboot_file() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./Xflash/Modified/gameboot.pmf", PSP_O_RDONLY, 0777);
	pmf_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/gameboot.pmf", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,pmf_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      APPLY CUSTOM GAMEBOOT ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,pmf_file_size);
	sceIoClose(fte);

}

void verify_user_gameboot() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,pmf_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<pmf_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/gameboot.pmf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,pmf_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_user_gameboot();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_forjan() {
	int ltff;
	int ftxr;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);


if (oea == 1) {		// force correct background size for 3.02 OE
bmp_file_size = 6176;
}

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);




if (oea == 1) {
	int cnt;
	cnt = 0;
	int jay;

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}

	for(jay=0; jay<6176; jay++) {
	allpics[cnt] = fntram[jay];
	cnt++;
	}
}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}
if (oea == 0) {
	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
} // oea
if (oea == 1) {
	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
} // oea
if (oea == 0) {
	sceIoWrite(fte,fntram,bmp_file_size);
}
if (oea == 1) {
	sceIoWrite(fte,allpics,74112);
}
	sceIoClose(fte);

}

void verify_background_forjan() {
	int uvf;
	int vpp;

if (oea == 0) {
	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/01.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01.bmp", PSP_O_RDONLY, 0777);
	}
}// oea

if (oea == 1) {
	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_RDONLY, 0777);
	}
}// oea
if (oea == 0) {
	sceIoRead(fxvf,vfntram,bmp_file_size);
}
if (oea == 1) {
	sceIoRead(fxvf,vfntram,74112);
}
	sceIoClose(fxvf);

	verify = 0;

if (oea == 0) {
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}
} // oea
if (oea == 1) {
	for(vpp=0; vpp<74112; vpp++) {
		if (allpics[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}
} // oea

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

if (oea == 0) {
	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
} // oea

if (oea == 1) {
	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/01-12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
} // oea

if (oea == 0) {
		sceIoWrite(fdo,fntram,bmp_file_size);
}
if (oea == 1) {
		sceIoWrite(fdo,allpics,74112);
}
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_forjan();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_forfeb() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_forfeb() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/02.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/02.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/02.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_forfeb();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_formar() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_formar() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/03.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/03.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/03.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_formar();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_forapr() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_forapr() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/04.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/04.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/04.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_forapr();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_formay() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_formay() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/05.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/05.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/05.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_formay();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_forjun() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_forjun() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/06.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/06.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/06.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_forjun();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_forjul() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_forjul() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/07.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/07.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/07.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_forjul();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_foraug() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_foraug() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/08.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/08.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/08.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_foraug();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_forsep() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_forsep() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/09.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/09.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/09.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_forsep();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_foroct() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_foroct() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/10.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/10.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/10.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_foroct();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_fornov() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_fornov() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/11.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/11.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/11.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_fornov();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void read_background_fordec() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;

	ffls = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	bmp_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	for(ftxr=0; ftxr<10; ftxr++) { // delay between access of same file
	sceDisplayWaitVblankStart();
	}

	ltff = sceIoOpen("./xflash/Modified/background.bmp", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,bmp_file_size);
	sceIoClose(ltff);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     APPLY CUSTOM BACKGROUND ");
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,bmp_file_size);
	sceIoClose(fte);

}

void verify_background_fordec() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/12.bmp", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/12.bmp", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,bmp_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<bmp_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/12.bmp", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,bmp_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_background_fordec();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void retmsc() {
	int fdh;

	syscon[130542] = 0x22;//
	syscon[130543] = 0x21;//

	syscon[130634] = 0x22;//
	syscon[130635] = 0x21;//

	syscon[131178] = 0x22;//
	syscon[131179] = 0x21;//

	syscon[131538] = 0x22;//
	syscon[131539] = 0x21;//

	syscon[131620] = 0x22;//
	syscon[131621] = 0x21;//

	syscon[131684] = 0x22;//
	syscon[131685] = 0x21;//

	syscon[131880] = 0x22;//
	syscon[131881] = 0x21;//

	syscon[133018] = 0x22;//
	syscon[133019] = 0x21;//

	print_blankfooter();

		if (test == 0) {
		fdh = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdh = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fdh,syscon,sysconfsize);
	sceIoClose(fdh);
}

void detmsc() {
	int fdg;

	syscon[130542] = 0x00;//
	syscon[130543] = 0x00;//

	syscon[130634] = 0x00;//
	syscon[130635] = 0x00;//

	syscon[131178] = 0x00;//
	syscon[131179] = 0x00;//

	syscon[131538] = 0x00;//
	syscon[131539] = 0x00;//

	syscon[131620] = 0x00;//
	syscon[131621] = 0x00;//

	syscon[131684] = 0x00;//
	syscon[131685] = 0x00;//

	syscon[131880] = 0x00;//
	syscon[131881] = 0x00;//

	syscon[133018] = 0x00;//
	syscon[133019] = 0x00;//

	print_blankfooter();
if (restore == 0 ) { // skip writing if auto restore status is set
		if (test == 0) {
		fdg = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdg = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fdg,syscon,sysconfsize);
	sceIoClose(fdg);
}
}

void xbasp() {
	int fdh;
	int cnt;
	int cntx = 0;
	int xnod;

	for(cnt=273541; cnt<313627; cnt++) {
	syscon[cnt] = basp[cntx];
	cntx++;
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    APPLY PLAIN BLACK SCREENS ");
	for(xnod=0; xnod<110; xnod++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		fdh = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdh = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fdh,syscon,sysconfsize);
	sceIoClose(fdh);
}

void xoasp() {
	int fdh;
	int cnt;
	int cntx = 0;
	int xnod;

	for(cnt=273541; cnt<313627; cnt++) {
	syscon[cnt] = oasp[cntx];
	cntx++;
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    RESTORE ORIGINAL SCREENS  ");
	for(xnod=0; xnod<110; xnod++) {
	sceDisplayWaitVblankStart();
	}

		if (test == 0) {
		fdh = sceIoOpen("flash0:/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdh = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/sysconf_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(fdh,syscon,sysconfsize);
	sceIoClose(fdh);
}

void retmtm() {
	int xnod;

	opening[14694] = 0x22;//
	opening[14695] = 0x21;//

	opening[14724] = 0x22;//
	opening[14725] = 0x21;//

	opening[16738] = 0x22;//
	opening[16739] = 0x21;//

	opening[16898] = 0x22;//
	opening[16899] = 0x21;//

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    RESTORE TRADEMARK SYMBOLS ");
	for(xnod=0; xnod<110; xnod++) {
	sceDisplayWaitVblankStart();
	}

	int voj;
	int vom;

		if (test == 0) {
		vom = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		vom = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(vom,opening,topmenusize);
	sceIoClose(vom);

	for(voj=0; voj<100; voj++) {
	sceDisplayWaitVblankStart();
	}
}

void detmtm() {
	int xnod;

	opening[14694] = 0x00;//
	opening[14695] = 0x00;//

	opening[14724] = 0x00;//
	opening[14725] = 0x00;//

	opening[16738] = 0x00;//
	opening[16739] = 0x00;//

	opening[16898] = 0x00;//
	opening[16899] = 0x00;//

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     REMOVE TRADEMARK SYMBOLS ");
	for(xnod=0; xnod<110; xnod++) {
	sceDisplayWaitVblankStart();
	}

	int voj;
	int vom;

		if (test == 0) {
		vom = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		vom = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

	sceIoWrite(vom,opening,topmenusize);
	sceIoClose(vom);

	for(voj=0; voj<100; voj++) {
	sceDisplayWaitVblankStart();
	}
}

void translator() {

if (easter == 1) {  // trigger easter egg 1337 translator
if (crbuff == 0x41) {
crbuff = 0x34;
}
if (crbuff == 0x61) {
crbuff = 0x34;
}
if (crbuff == 0x42) {
crbuff = 0x38;
}
if (crbuff == 0x62) {
crbuff = 0x38;
}
if (crbuff == 0x43) {
crbuff = 0x28;
}
if (crbuff == 0x63) {
crbuff = 0x28;
}
if (crbuff == 0x45) {
crbuff = 0x33;
}
if (crbuff == 0x65) {
crbuff = 0x33;
}
if (crbuff == 0x47) {
crbuff = 0x36;
}
if (crbuff == 0x67) {
crbuff = 0x36;
}
if (crbuff == 0x4C) {
crbuff = 0x31;
}
if (crbuff == 0x6C) {
crbuff = 0x31;
}
if (crbuff == 0x4F) {
crbuff = 0x30;
}
if (crbuff == 0x6F) {
crbuff = 0x30;
}
if (crbuff == 0x53) {
crbuff = 0x35;
}
if (crbuff == 0x73) {
crbuff = 0x35;
}
if (crbuff == 0x54) {
crbuff = 0x37;
}
if (crbuff == 0x74) {
crbuff = 0x37;
}
} // end of easter egg code
}

void print_starline() {
	int strx = 5;
//	int stari;

if (fsshow == 0) {
if (mcnt > 245) {
if (mcnt < 265) {
move_starscreen();
}
}
if (mcnt > 295) {
if (mcnt < 309) {
movx_starscreen();
}
}
if (mcnt > 310) {
if (mcnt < 329) {
move_starscreen();
}
}
if (mcnt > 330) {
if (mcnt < 339) {
movx_starscreen();
}
}
if (mcnt > 340) {
if (mcnt < 349) {
move_starscreen();
}
}
if (mcnt > 368) {
if (mcnt < 379) {
movx_starscreen();
}
}
if (mcnt > 380) {
if (mcnt < 410) {
move_starscreen();
}
}
if (mcnt > 450) {
if (mcnt < 464) {
movx_starscreen();
}
}

}
else {
if (fs_down == 1) {
move_starscreen();
}
if (fs_up == 1) {
movx_starscreen();
}
}

pspDebugScreenSetTextColor(0x00EFDDCA);

if (fs_right == 0) {
if (fs_left == 0) {
if (fsshow == 1) {
fs_right = 1;
}
if (fsshow == 0) {
fs_left = 1;
fs_right = 0;
}
}
}

if (fs_left == 1) {

	stra = stra - 60;
	pspDebugScreenSetXY(strx, starda);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[stra] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	stra++;
	if (stra < 60) {
	stra = 463;
	}
	}

	strb = strb - 57;
	pspDebugScreenSetXY(strx, stardb);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strb] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strb++;
	if (strb < 60) {
	strb = 477;
	}
	}

	strc = strc - 58;
	pspDebugScreenSetXY(strx, stardc);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strc] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strc++;
	if (strc < 60) {
	strc = 481;
	}
	}

	strd = strd - 59;
	pspDebugScreenSetXY(strx, stardd);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strd] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 445) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strd++;
	if (strd < 60) {
	strd = 492;
	}
	}

	stre = stre - 58;
	pspDebugScreenSetXY(strx, starde);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[stre] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	stre++;
	if (stre < 60) {
	stre = 478;
	}
	}

	strf = strf - 59;
	pspDebugScreenSetXY(strx, stardf);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strf] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strf++;
	if (strf < 60) {
	strf = 474;
	}
	}

	strg = strg - 57;
	pspDebugScreenSetXY(strx, stardg);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strg] ^ 0x3F);	//decrypt simple xor encrypted cedit


if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strg++;
	if (strg < 60) {
	strg = 488;
	}
	}

	strh = strh - 59;
	pspDebugScreenSetXY(strx, stardh);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strh] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strh++;
	if (strh < 60) {
	strh = 478;
	}
	}

	stri = stri - 58;
	pspDebugScreenSetXY(strx, stardi);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[stri] ^ 0x3F);	//decrypt simple xor encrypted cedit


if (mcnt > 445) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	stri++;
	if (stri < 60) {
	stri = 482;
	}
	}

	strj = strj - 57;
	pspDebugScreenSetXY(strx, stardj);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strj] ^ 0x3F);	//decrypt simple xor encrypted cedit


if (mcnt > 445) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strj++;
	if (strj < 60) {
	strj = 470;
	}
	}

	strk = strk - 59;
	pspDebugScreenSetXY(strx, stardk);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strk] ^ 0x3F);	//decrypt simple xor encrypted cedit


if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strk++;
	if (strk < 60) {
	strk = 475;
	}
	}

	strl = strl - 58;
	pspDebugScreenSetXY(strx, stardl);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strl] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strl++;
	if (strl < 60) {
	strl = 495;
	}
	}

	strm = strm - 59;
	pspDebugScreenSetXY(strx, stardm);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strm] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strm++;
	if (strm < 60) {
	strm = 485;
	}
	}

	strn = strn - 58;
	pspDebugScreenSetXY(strx, stardn);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strn] ^ 0x3F);	//decrypt simple xor encrypted credits


if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}
	printf("%c",stbuff);
	strn++;
	if (strn < 60) {
	strn = 490;
	}
	}

	stro = stro - 60;
	pspDebugScreenSetXY(strx, stardo);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[stro] ^ 0x3F);	//decrypt simple xor encrypted credits

if (mcnt > 445) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}


	printf("%c",stbuff);
	stro++;
	if (stro < 60) {
	stro = 480;
	}
	}

} // fs_left = 1


if (fs_right == 1) {
	stra = stra - 52;
	pspDebugScreenSetXY(strx, starda);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[stra] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	stra++;
	if (stra > 462) {
	stra = 60;
	}
	}

	strb = strb - 55;
	pspDebugScreenSetXY(strx, stardb);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strb] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strb++;
	if (strb > 476) {
	strb = 60;
	}
	}

	strc = strc - 54;
	pspDebugScreenSetXY(strx, stardc);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strc] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strc++;
	if (strc > 480) {
	strc = 60;
	}
	}

	strd = strd - 53;
	pspDebugScreenSetXY(strx, stardd);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strd] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 445) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strd++;
	if (strd > 491) {
	strd = 60;
	}
	}

	stre = stre - 54;
	pspDebugScreenSetXY(strx, starde);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[stre] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	stre++;
	if (stre > 477) {
	stre = 60;
	}
	}

	strf = strf - 53;
	pspDebugScreenSetXY(strx, stardf);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strf] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strf++;
	if (strf > 473) {
	strf = 60;
	}
	}

	strg = strg - 55;
	pspDebugScreenSetXY(strx, stardg);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strg] ^ 0x3F);	//decrypt simple xor encrypted cedit


if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strg++;
	if (strg > 487) {
	strg = 60;
	}
	}

	strh = strh - 53;
	pspDebugScreenSetXY(strx, stardh);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strh] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strh++;
	if (strh > 477) {
	strh = 60;
	}
	}

	stri = stri - 54;
	pspDebugScreenSetXY(strx, stardi);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[stri] ^ 0x3F);	//decrypt simple xor encrypted cedit


if (mcnt > 445) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	stri++;
	if (stri > 481) {
	stri = 60;
	}
	}

	strj = strj - 55;
	pspDebugScreenSetXY(strx, stardj);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strj] ^ 0x3F);	//decrypt simple xor encrypted cedit


if (mcnt > 445) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strj++;
	if (strj > 469) {
	strj = 60;
	}
	}

	strk = strk - 53;
	pspDebugScreenSetXY(strx, stardk);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strk] ^ 0x3F);	//decrypt simple xor encrypted cedit


if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strk++;
	if (strk > 474) {
	strk = 60;
	}
	}

	strl = strl - 54;
	pspDebugScreenSetXY(strx, stardl);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strl] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strl++;
	if (strl > 494) {
	strl = 60;
	}
	}

	strm = strm - 53;
	pspDebugScreenSetXY(strx, stardm);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strm] ^ 0x3F);	//decrypt simple xor encrypted cedit

if (mcnt > 440) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}

	printf("%c",stbuff);
	strm++;
	if (strm > 484) {
	strm = 60;
	}
	}

	strn = strn - 54;
	pspDebugScreenSetXY(strx, stardn);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[strn] ^ 0x3F);	//decrypt simple xor encrypted credits


if (mcnt > 435) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}
	printf("%c",stbuff);
	strn++;
	if (strn > 489) {
	strn = 60;
	}
	}

	stro = stro - 52;
	pspDebugScreenSetXY(strx, stardo);
	printf(" ");
	for(stari=0; stari<56; stari++) {
	stbuff = (stars[stro] ^ 0x3F);	//decrypt simple xor encrypted credits

if (mcnt > 445) {
if (stbuff == 0x2E) {
stbuff = 0x6F;
}
}
	printf("%c",stbuff);
	stro++;
	if (stro > 479) {
	stro = 60;
	}
	}
} // fs_right = 1
}

void move_starscreen() {
starbb = stardo;
stardo = stardn;
stardn = stardm;
stardm = stardl;
stardl = stardk;
stardk = stardj;
stardj = stardi;
stardi = stardh;
stardh = stardg;
stardg = stardf;
stardf = starde;
starde = stardd;
stardd = stardc;
stardc = stardb;
stardb = starda;
starda = starbb;
}

void movx_starscreen() {
starbb = starda;
starda = stardb;
stardb = stardc;
stardc = stardd;
stardd = starde;
starde = stardf;
stardf = stardg;
stardg = stardh;
stardh = stardi;
stardi = stardj;
stardj = stardk;
stardk = stardl;
stardl = stardm;
stardm = stardn;
stardn = stardo;
stardo = starbb;
}

void check_gameboot() {
	musac = sceIoOpen("./xflash/Modified/gameboot.pmf", PSP_O_RDONLY, 0777);
	file_size = sceIoLseek32(musac, 0, SEEK_END);
	sceIoClose(musac);

	musac = sceIoOpen("./xflash/Modified/gameboot.pmf", PSP_O_RDONLY, 0777);
	sceIoRead(musac,gbram,0x5);
	sceIoClose(musac);
}
void check_music() {
	musac = sceIoOpen("./xflash/Xflash/sndcx.XF", PSP_O_RDONLY, 0777);
	sceIoRead(musac,gbram,0x133);
	sceIoClose(musac);

	musac = sceIoOpen("./xflash/Xflash/sndcx.XF", PSP_O_RDONLY, 0777);
	file_size = sceIoLseek32(musac, 0, SEEK_END);
	sceIoClose(musac);
}

void flash_title() {
	int j;
	pspDebugScreenSetTextColor(hilight);
	for(j=0; j<1; j++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(29, 6);
	printf(">");
	for(j=0; j<1; j++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(30, 6);
	printf("<");
	for(j=0; j<1; j++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(31, 6);
	printf("-");
	for(j=0; j<1; j++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(32, 6);
	printf("F");
	for(j=0; j<1; j++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(33, 6);
	printf("l");
	for(j=0; j<1; j++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(34, 6);
	printf("a");
	for(j=0; j<1; j++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(35, 6);
	printf("s");
	for(j=0; j<1; j++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(36, 6);
	printf("h");
	for(j=0; j<8; j++) {
	sceDisplayWaitVblankStart();
	}
}

void delete_song() {
	if (test == 0) {
	sceIoRemove("flash0:/Song.mp3");
	}
	else {
	sceIoRemove("./Xflash/Dump/flash0/Song.mp3");
	}
exist = 1;
}

void check_power() {
	int jpwr;

	while (1) {
		isplugged = scePowerIsPowerOnline();
		if (isplugged == 1) {
		break; // power cable was detected
		}

		if(scePowerIsLowBattery()) {
		sceDisplayWaitVblankStart();
		isbatlow = 1;
		}
		if (isbatlow == 0) {
		break; // battery level is ok
		}
		else {
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     BATTERY LEVEL IS LOW       ");
	for(jpwr=0; jpwr<250; jpwr++) {
	sceDisplayWaitVblankStart();
	}
	boot_power();
    pspAudioEnd();
	sceKernelExitGame();
	}
	}
}

void vshmaincheck() {
	int chmn;

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf(" CHECK FOR MOD ALREADY APPLIED");
	for(chmn=0; chmn<110; chmn++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	chmn = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}
	else {
	chmn = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}

	vshmainsize = sceIoLseek32(chmn, 0, SEEK_END);
	sceIoClose(chmn);

if (vshmainsize != 72032) {
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  VSHMAIN.PRX IS NOT ORIGINAL!");
	for(chmn=0; chmn<160; chmn++) {
	sceDisplayWaitVblankStart();
	}
exitdarkalex = 1; // dont do the mod twice
}

if (vshmainsize == 30368) {
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("   NO POINT IN DOING IT TWICE ");
	for(chmn=0; chmn<170; chmn++) {
	sceDisplayWaitVblankStart();
	}
}
}

void read_vshmain() {
	int xsp;
	int fdg;
	int xnod;

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    APPLY CUSTOM FIRMWARE MOD ");
	for(xnod=0; xnod<90; xnod++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(18, 29);
	printf("            Dark_AleX         ");
	for(xnod=0; xnod<110; xnod++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	xsp = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}
	else {
	xsp = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}

	sceIoRead(xsp,vshreal,72032);
	sceIoClose(xsp);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 29);
	printf("                              ");
	pspDebugScreenSetXY(18, 28);
	printf("    COPY ORIGINAL VSHMAIN.PRX ");
	for(xnod=0; xnod<110; xnod++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fdg = sceIoOpen("flash0:/vsh/module/vshmain_real.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdg = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain_real.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fdg,vshreal,72032);
	sceIoClose(fdg);
}

void verify_vshmain() {
	int fxvs;
	int vsa;
	int xsv;

	if (test == 0) {
	fxvs = sceIoOpen("flash0:/vsh/module/vshmain_real.prx", PSP_O_RDONLY, 0777);
	}
	else {
	fxvs = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain_real.prx", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvs,vvshreal,72032);
	sceIoClose(fxvs);

	verify = 0;
	for(vsa=0; vsa<72032; vsa++) {
		if (vshreal[vsa] != vvshreal[vsa]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/module/vshmain_real.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain_real.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,vshreal,72032);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_vshmain();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(xsv=0; xsv<60; xsv++) {
	sceDisplayWaitVblankStart();
	}
exist = 1;
}

void vshfaketoram() {
	int gbtr;
	int gbk;
	int qmo;

	for(qmo=0; qmo<30368; qmo++) {
		gbram[qmo] = vshmain[qmo];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    APPLY CUSTOM VSHMAIN.PRX   ");

	for(gbtr=0; gbtr<110; gbtr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	gbk = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	gbk = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(gbk,gbram,30368);
	sceIoClose(gbk);
}

void verify_vshfake() {
	int ugb;
	int fxgb;
	int tpl;

	if (test == 0) {
	fxgb = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}
	else {
	fxgb = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxgb,gbram,30368);

	sceIoClose(fxgb);

	verify = 0;
	for(tpl=0; tpl<30368; tpl++) {

		if (gbram[tpl] != vshmain[tpl]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,vshmain,30368);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_vshfake();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(ugb=0; ugb<60; ugb++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void recoverytoram() {
	int gbtr;
	int gbk;
	int qmo;

	for(qmo=0; qmo<40144; qmo++) {
		gbram[qmo] = recovery[qmo];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("       APPLY RECOVERY.PRX      ");
	for(gbtr=0; gbtr<110; gbtr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	gbk = sceIoOpen("flash0:/kd/recovery.elf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	gbk = sceIoOpen("./Xflash/Dump/flash0/kd/recovery.elf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(gbk,gbram,40144);
	sceIoClose(gbk);
}

void verify_recovery() {
	int ugb;
	int fxgb;
	int tpl;

	if (test == 0) {
	fxgb = sceIoOpen("flash0:/kd/recovery.elf", PSP_O_RDONLY, 0777);
	}
	else {
	fxgb = sceIoOpen("./Xflash/Dump/flash0/kd/recovery.elf", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxgb,gbram,40144);
	sceIoClose(fxgb);

	verify = 0;
	for(tpl=0; tpl<40144; tpl++) {

		if (gbram[tpl] != recovery[tpl]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/kd/recovery.elf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/kd/recovery.elf", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,recovery,40144);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_recovery();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(ugb=0; ugb<60; ugb++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void configtoram() {
	int gbtr;
	int gbk;
	int qmo;

	pspDebugScreenSetXY(18, 28);
	printf("    CHECK PSP/SYSTEM FOLDER    ");
	for(gbtr=0; gbtr<60; gbtr++) {
	sceDisplayWaitVblankStart();
	}


	int dfd;
	dfd = sceIoDopen("ms0:/PSP/SYSTEM/");
	if(dfd >= 0) {
	sysfolderexists = 1;
	}

	pspDebugScreenSetXY(18, 28);
if (sysfolderexists == 1) {
	printf("    PSP/SYSTEM FOLDER EXISTS  ");
}
else {
	printf("    CREATE SYSTEM/PSP FOLDER  ");
}
	for(gbtr=0; gbtr<10; gbtr++) {
	sceDisplayWaitVblankStart();
	}

if (sysfolderexists == 0) {
sceIoMkdir("ms0:/PSP/SYSTEM", 0777);
}
	for(gbtr=0; gbtr<130; gbtr++) {
	sceDisplayWaitVblankStart();
	}

	for(qmo=0; qmo<290; qmo++) {
		gbram[qmo] = config[qmo];
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("   CREATE CONFIGURATION FILE   ");
	for(gbtr=0; gbtr<110; gbtr++) {
	sceDisplayWaitVblankStart();
	}

	gbk = sceIoOpen("ms0:/PSP/SYSTEM/config.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

	sceIoWrite(gbk,gbram,290);
	sceIoClose(gbk);
}

void verify_config() {
	int ugb;
	int fxgb;
	int tpl;

	fxgb = sceIoOpen("ms0:/PSP/SYSTEM/config.txt", PSP_O_RDONLY, 0777);

	sceIoRead(fxgb,gbram,290);
	sceIoClose(fxgb);

	verify = 0;
	for(tpl=0; tpl<290; tpl++) {

		if (gbram[tpl] != config[tpl]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	fdo = sceIoOpen("ms0:/PSP/SYSTEM/config.txt", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

		sceIoWrite(fdo,config,290);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_config();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(ugb=0; ugb<60; ugb++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void vshmaincheckrev() {
	int chmn;

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("CHECK FOR MOD ALREADY REVERSED");
	for(chmn=0; chmn<110; chmn++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	chmn = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}
	else {
	chmn = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}

	vshmainsize = sceIoLseek32(chmn, 0, SEEK_END);
	sceIoClose(chmn);

if (vshmainsize == 72032) {
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("     VSHMAIN.PRX IS ORIGINAL  ");
	for(chmn=0; chmn<160; chmn++) {
	sceDisplayWaitVblankStart();
	}
exitdarkalexrev = 1; // dont reverse the mod twice
}
}

void read_vshmainrev() {
	int xsp;
	int fdg;
	int xnod;

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("  REVERSE CUSTOM FIRMWARE MOD ");
	for(xnod=0; xnod<90; xnod++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	xsp = sceIoOpen("flash0:/vsh/module/vshmain_real.prx", PSP_O_RDONLY, 0777);
	}
	else {
	xsp = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain_real.prx", PSP_O_RDONLY, 0777);
	}

	sceIoRead(xsp,vshreal,72032);
	sceIoClose(xsp);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 29);
	printf("                              ");
	pspDebugScreenSetXY(18, 28);
	printf("    COPY ORIGINAL VSHMAIN.PRX ");
	for(xnod=0; xnod<110; xnod++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fdg = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdg = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fdg,vshreal,72032);
	sceIoClose(fdg);
}

void verify_vshmainrev() {
	int fxvs;
	int vsa;
	int xsv;

	if (test == 0) {
	fxvs = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}
	else {
	fxvs = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvs,vvshreal,72032);
	sceIoClose(fxvs);

	verify = 0;
	for(vsa=0; vsa<72032; vsa++) {
		if (vshreal[vsa] != vvshreal[vsa]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,vshreal,72032);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_vshmainrev();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(xsv=0; xsv<60; xsv++) {
	sceDisplayWaitVblankStart();
	}
exist = 1;
}

void delete_vshmainreal() {
	int dexg;

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("   DELETE RENAMED VSHMAIN.PRX ");
	for(dexg=0; dexg<110; dexg++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	sceIoRemove("flash0:/vsh/module/vshmain_real.prx");
	}
	else {
	sceIoRemove("./Xflash/Dump/flash0/vsh/module/vshmain_real.prx");
	}
}

void verify_vshmainrealgone() {
	int pmfvf;
	int fdv;

	if (test == 0) {
	fdv = sceIoOpen("flash0:/vsh/module/vshmain_real.prx", PSP_O_RDONLY, 0);
	}
	else {
	fdv = sceIoOpen("./Xflash/Dump/flash0/vsh/module/vshmain_real.prx", PSP_O_RDONLY, 0);
	}

	if(fdv < 0) {
		exist = 0;
        }
	else {
                exist = 1;
        }
	sceIoClose(fdv);

	verify = 0;
	if (exist == 1) {
	verify = 1;
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		delete_vshmainreal();

		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_vshmainrealgone();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(pmfvf=0; pmfvf<60; pmfvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void delete_recovery() {
	int desg;

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("       DELETE RECOVERY.ELF    ");
	for(desg=0; desg<110; desg++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	sceIoRemove("flash0:/kd/recovery.elf");
	}
	else {
	sceIoRemove("./Xflash/Dump/flash0/kd/recovery.elf");
	}
}

void verify_recoverygone() {
	int pmfvf;
	int fdv;

	if (test == 0) {
	fdv = sceIoOpen("flash0:/kd/recovery.elf", PSP_O_RDONLY, 0);
	}
	else {
	fdv = sceIoOpen("./Xflash/Dump/flash0/kd/recovery.elf", PSP_O_RDONLY, 0);
	}

	if(fdv < 0) {
		exist = 0;
        }
	else {
                exist = 1;
        }
	sceIoClose(fdv);

	verify = 0;
	if (exist == 1) {
	verify = 1;
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		delete_recovery();

		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_recoverygone();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(pmfvf=0; pmfvf<60; pmfvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void delete_configuration() {
	int desg;
	int fdvc;

	fdvc = sceIoOpen("ms0:/PSP/SYSTEM/config.txt", PSP_O_RDONLY, 0); //  *****check for config.txt existence *****

	if(fdvc < 0) {
		there = 0;
        }
	else {
                there = 1;
        }
	sceIoClose(fdvc);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
if (there == 0) {
	printf("  CONFIG.TXT DOES NOT EXIST  ");
	for(desg=0; desg<110; desg++) {
	sceDisplayWaitVblankStart();
	}
}
else {
	printf("      CONFIG.TXT FOUND       ");
	for(desg=0; desg<110; desg++) {
	sceDisplayWaitVblankStart();
	}
}

if (there == 1) {
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("    DELETE SYSTEM/CONFIG.TXT  ");
	for(desg=0; desg<110; desg++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("ms0:/PSP/SYSTEM/config.txt");
}
}

void verify_configurationgone() {
	int pmfvf;
	int fdv;

	if (there == 1) {

	fdv = sceIoOpen("ms0:/PSP/SYSTEM/config.txt", PSP_O_RDONLY, 0);

	if(fdv < 0) {
		exist = 0;
        }
	else {
                exist = 1;
        }
	sceIoClose(fdv);

	verify = 0;
	if (exist == 1) {
	verify = 1;
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		delete_configuration();

		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_configurationgone();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(pmfvf=0; pmfvf<60; pmfvf++) {
	sceDisplayWaitVblankStart();
	}

	}
	there = 0;
	exist = 1;
}

void read_name_plate() {
	int ltff;
	int ftxr;
	int fte;
	int ffls;
	int count;

	if (art == 0) {
	ffls = sceIoOpen("./xflash/Modified/name_plate.png", PSP_O_RDONLY, 0777);
	font_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	ltff = sceIoOpen("./xflash/Modified/name_plate.png", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,font_file_size);
	sceIoClose(ltff);
	}

	if (art == 1) {
	if (seb == 0) {
	font_file_size = 33999; // set file size of Easter Egg splash screen
	for(count=0; count<33999; count++) {
	fntram[count] = (artplate[count] ^ 0x3F); // simple Xor decrypt of Easter Egg splash screen
	}
	}

	if (seb == 1) {
	ffls = sceIoOpen("./xflash/Modified/name_plate.png", PSP_O_RDONLY, 0777);
	font_file_size = sceIoLseek32(ffls, 0, SEEK_END);
	sceIoClose(ffls);

	ltff = sceIoOpen("./xflash/Modified/name_plate.png", PSP_O_RDONLY, 0777);
	sceIoRead(ltff,fntram,font_file_size);
	sceIoClose(ltff);
	}
	}

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	if (art == 0) {
	printf("     APPLY CUSTOM NAME PLATE");
	}
	if (art == 1) {
	if (seb == 0) {
	printf("   APPLY EASTER EGG NAME PLATE");
	}
	if (seb == 1) {
	printf("     APPLY CUSTOM NAME PLATE");
	}
	}
	for(ftxr=0; ftxr<110; ftxr++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	fte = sceIoOpen("flash0:/vsh/resource/name_plate.png", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fte = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/name_plate.png", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

	sceIoWrite(fte,fntram,font_file_size);
	sceIoClose(fte);

}

void verify_name_plate() {
	int uvf;
	int fxvf;
	int vpp;

	if (test == 0) {
	fxvf = sceIoOpen("flash0:/vsh/resource/name_plate.png", PSP_O_RDONLY, 0777);
	}
	else {
	fxvf = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/name_plate.png", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fxvf,vfntram,font_file_size);
	sceIoClose(fxvf);

	verify = 0;
	for(vpp=0; vpp<font_file_size; vpp++) {
		if (fntram[vpp] != vfntram[vpp]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

	if (test == 0) {
	fdo = sceIoOpen("flash0:/vsh/resource/name_plate.png", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}
	else {
	fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/name_plate.png", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	}

		sceIoWrite(fdo,fntram,font_file_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_name_plate();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animC();
	print_verify_passed();
	}

	for(uvf=0; uvf<60; uvf++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void delete_name_plate() {
	int desg;
	int fdvc;

	if (test == 0) {
	fdvc = sceIoOpen("flash0:/vsh/resource/name_plate.png", PSP_O_RDONLY, 0); //  *****check for name_plate.png existence *****
	}
	else {
	fdvc = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/name_plate.png", PSP_O_RDONLY, 0777);
	}

	if(fdvc < 0) {
		there = 0;
        }
	else {
                there = 1;
        }
	sceIoClose(fdvc);

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
if (there == 0) {
	printf("NAME_PLATE.PNG DOES NOT EXIST");
	for(desg=0; desg<110; desg++) {
	sceDisplayWaitVblankStart();
	}
}
else {
	printf("    NAME_PLATE.PNG FOUND     ");
	for(desg=0; desg<110; desg++) {
	sceDisplayWaitVblankStart();
	}
}

if (there == 1) {
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 28);
	printf("      DELETE NAME_PLATE.PNG   ");
	for(desg=0; desg<110; desg++) {
	sceDisplayWaitVblankStart();
	}

	if (test == 0) {
	sceIoRemove("flash0:/vsh/resource/name_plate.png");
	}
	else {
	sceIoRemove("./Xflash/Dump/flash0/vsh/resource/name_plate.png");
	}
}
}

void verify_name_plate_gone() {
	int pmfvf;
	int fdv;

if (there == 1) {
	if (test == 0) {
	fdv = sceIoOpen("flash0:/vsh/resource/name_plate.png", PSP_O_RDONLY, 0); //  *****check for name_plate.png existence *****
	}
	else {
	fdv = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/name_plate.png", PSP_O_RDONLY, 0777);
	}

	if(fdv < 0) {
		exist = 0;
        }
	else {
                exist = 1;
        }
	sceIoClose(fdv);

	verify = 0;
	if (exist == 1) {
	verify = 1;
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		delete_configuration();

		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_name_plate_gone();
	}
	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(pmfvf=0; pmfvf<60; pmfvf++) {
	sceDisplayWaitVblankStart();
	}

}
	there = 0;
	exist = 1;
}

void print_exit() {
	int p;
	pspDebugScreenSetXY(0, 28);
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("      ;`-';                ");
	pspDebugScreenSetTextColor(colord);
	printf(" EXIT PROGRAM ");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("                ;`-';");
	for(p=0; p<30; p++) {
	sceDisplayWaitVblankStart();
	}
}

void another_function() {
	int s;
	test = 1;	// set test mode status
	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(18, 27);
	printf("          TEST MODE ON ");
	pspDebugScreenSetXY(18, 29);
	printf("     CHICKEN THEME ACTIVATED   ");
	for(s=0; s<70; s++) {
	sceDisplayWaitVblankStart();
	}
}

void print_delete_song() {
	pspDebugScreenSetXY(0, 28);
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("      ;`-';                ");
	pspDebugScreenSetTextColor(colord);
	printf(" DELETE SONG  ");
	pspDebugScreenSetTextColor(colorb); // Blueish for header or footer
	printf("                ;`-';");

}

void ifgameisstuff() {
			if (game == 1) {
			txtcode();
			}
			if (game == 2) {
			txtexec();
			}
			if (game == 3) {
			txtgame();
			}
			if (game == 4) {
			txtgamz();
			}
			if (game == 5) {
			txt1337();
			}
			if (game == 6) {
			txtfilz();
			}
			if (game == 7) {
			txtapps();
			}
			if (game == 8) {
			txt64mz();
			}
			if (game == 9) {
			txti505();
			}
			if (game == 10) {
			txttunes();
			}
			if (game == 11) {
			txtmusic();
			}
			if (game == 12) {
			txtaudio();
			}
			if (game == 13) {
			txtphoto();
			}
			if (game == 14) {
			txtporn();
			}
			if (game == 15) {
			txtsnaps();
			}
			if (game == 16) {
			txtmovie();
			}
			if (game == 17) {
			txtvideo();
			}
}

void finger_a() {
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	pspDebugScreenSetXY(0, 10);
	printf("\n                                                       ");
	printf("\n            ____                                       ");
	printf("\n          -'  __\\____                                  ");
	printf("\n                 ____)                                 ");
	printf("\n                 _)                                    ");
	printf("\n                _)                                     ");
	printf("\n          -.____)                                      ");
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n                                                       ");
}

void finger_b() {
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	pspDebugScreenSetXY(0, 10);
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                   _____     ");
	printf("\n                                             _____/ _   `-   ");
	printf("\n                                            (____'-'         ");
	printf("\n                                               (___)) _.     ");
	printf("\n                                                (__))`-      ");
	printf("\n                                                 (_))___.-   ");
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                             ");
}

void finger_c() {
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	pspDebugScreenSetXY(0, 10);
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n            ____                                       ");
	printf("\n          -'  __\\____                                  ");
	printf("\n                 ____)                                 ");
	printf("\n                 _)                                    ");
	printf("\n                _)                                     ");
	printf("\n          -.____)                                      ");
	printf("\n                                                       ");
	printf("\n                                                       ");
}

void finger_d() {
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	pspDebugScreenSetXY(0, 10);
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                             ");
	printf("\n                                                   _____     ");
	printf("\n                                             _____/ _   `-   ");
	printf("\n                                            (____'-'         ");
	printf("\n                                               (___)) _.     ");
	printf("\n                                                (__))`-      ");
	printf("\n                                                 (_))___.-   ");
}

void finger_patch() {
	int gdf;
	for(gdf= 12; gdf<22; gdf++) {
	pspDebugScreenSetXY(6, gdf);
	printf("            ");
}
}

void finger_ok_anim() {
finger_patch();
int aok;
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	for(aok= 0; aok<10; aok++) { // adjust thumbs up slide in/forearm length
	pspDebugScreenSetXY(aok, 12);
	printf("           ");
	pspDebugScreenSetXY(aok, 13);
	printf("       _   ");
	pspDebugScreenSetXY(aok, 14);
	printf("      / )  ");
	pspDebugScreenSetXY(aok, 15);
	printf("    .' /   ");
	pspDebugScreenSetXY(aok, 16);
	printf("---'  (___ ");
	pspDebugScreenSetXY(aok, 17);
	printf("        __)");
	pspDebugScreenSetXY(aok, 18);
	printf("        __)");
	pspDebugScreenSetXY(aok, 19);
	printf("       __) ");
	pspDebugScreenSetXY(aok, 20);
	printf("---.____)  ");
	pspDebugScreenSetXY(0, 16);
	printf("      ");
	pspDebugScreenSetXY(0, 20);
	printf("      ");
	sceDisplayWaitVblankStart();
}
}

void finger_ok_animC() {
finger_patch();
int aok;
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	for(aok= 0; aok<7; aok++) { // adjust thumbs up slide in/forearm length (27)
	pspDebugScreenSetXY(aok, 12);
	printf("            ");
	pspDebugScreenSetXY(aok, 13);
	printf("      _     ");
	pspDebugScreenSetXY(aok, 14);
	printf("     / )    ");
	pspDebugScreenSetXY(aok, 15);
	printf("   .' /     ");
	pspDebugScreenSetXY(aok, 16);
	printf("--'  (____  ");
	pspDebugScreenSetXY(aok, 17);
	printf("      ((__) ");
	pspDebugScreenSetXY(aok, 18);
	printf("   ._ ((___)");
	pspDebugScreenSetXY(aok, 19);
	printf("    -'((__) ");
	pspDebugScreenSetXY(aok, 20);
	printf("--.___((_)  ");
	pspDebugScreenSetXY(aok, 21);
	printf("            ");
	pspDebugScreenSetXY(aok, 22);
	printf("            ");
	pspDebugScreenSetXY(0, 16);
	printf("      ");
	pspDebugScreenSetXY(0, 20);
	printf("      ");
	sceDisplayWaitVblankStart();
	sceDisplayWaitVblankStart();
}
}

void finger_ok_animB() {
finger_patch();
int adk;
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	for(adk= 23; adk>11; adk--) { // adjust height that hand gesture raises
	pspDebugScreenSetXY(6, adk);
if (adk < 24) {
	printf("           ");
}
	pspDebugScreenSetXY(6, adk + 1);
if (adk < 23) {
	printf("       _ _ ");
}
	pspDebugScreenSetXY(6, adk + 2);
if (adk < 22) {
	printf("    .-/ / )");
}
	pspDebugScreenSetXY(6, adk + 3);
if (adk < 21) {
	printf("    |/ / / ");
}
	pspDebugScreenSetXY(6, adk + 4);
if (adk < 20) {
	printf("    /.' /  ");
}
	pspDebugScreenSetXY(6, adk + 5);
if (adk < 19) {
	printf("   // .---.");
}
	pspDebugScreenSetXY(6, adk + 6);
if (adk < 18) {
	printf("  /   .--._\\");
}
	pspDebugScreenSetXY(6, adk + 7);
if (adk < 17) {
	printf(" /    `--' /");
}
	pspDebugScreenSetXY(6, adk + 8);
if (adk < 16) {
	printf("/     .---' ");
}
	pspDebugScreenSetXY(6, adk + 9);
if (adk < 15) {
	printf("    .'     ");
}
	pspDebugScreenSetXY(6, adk + 10);
if (adk < 14) {
	printf("            ");
}
	pspDebugScreenSetXY(6, adk + 11);
if (adk < 13) {
	printf("            ");
}
	sceDisplayWaitVblankStart();
}
}

void finger_stop_anim() {
finger_patch();
int adk;
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	for(adk= 23; adk>10; adk--) { // adjust height that stop hand raises
	pspDebugScreenSetXY(6, adk);
if (adk < 24) {
	printf("    .-.     ");
}
	pspDebugScreenSetXY(6, adk + 1);
if (adk < 23) {
	printf("  .-| |-.   ");
}
	pspDebugScreenSetXY(6, adk + 2);
if (adk < 22) {
	printf("  | | | |   ");
}
	pspDebugScreenSetXY(6, adk + 3);
if (adk < 21) {
	printf("  | | | |-. ");
}
	pspDebugScreenSetXY(6, adk + 4);
if (adk < 20) {
	printf("  | | | | | ");
}
	pspDebugScreenSetXY(6, adk + 5);
if (adk < 19) {
	printf("  |_|_|_| | ");
}
	pspDebugScreenSetXY(6, adk + 6);
if (adk < 18) {
	printf(" / )    `-| ");
}
	pspDebugScreenSetXY(6, adk + 7);
if (adk < 17) {
	printf("| | `--   | ");
}
	pspDebugScreenSetXY(6, adk + 8);
if (adk < 16) {
	printf("|     ||  | ");
}
	pspDebugScreenSetXY(6, adk + 9);
if (adk < 15) {
	printf(" \\     ' /  ");
}
	pspDebugScreenSetXY(6, adk + 10);
if (adk < 14) {
	printf("  |      |  ");
}
	pspDebugScreenSetXY(6, adk + 11);
if (adk < 13) {
	printf("            ");
}
	sceDisplayWaitVblankStart();
}
}

void flip_finger() {
finger_patch();
int adk;
	grey = 0x00EEEEEE;
	pspDebugScreenSetTextColor(grey);
	for(adk= 23; adk>10; adk--) { // adjust height that stop hand raises
	pspDebugScreenSetXY(6, adk);
if (adk < 24) {
	printf("     .-.    ");
}
	pspDebugScreenSetXY(6, adk + 1);
if (adk < 23) {
	printf("     |U|    ");
}
	pspDebugScreenSetXY(6, adk + 2);
if (adk < 22) {
	printf("     | |    ");
}
	pspDebugScreenSetXY(6, adk + 3);
if (adk < 21) {
	printf("     | |    ");
}
	pspDebugScreenSetXY(6, adk + 4);
if (adk < 20) {
	printf("    _| |_   ");
}
	pspDebugScreenSetXY(6, adk + 5);
if (adk < 19) {
	printf("   | | | |-.");
}
	pspDebugScreenSetXY(6, adk + 6);
if (adk < 18) {
	printf("  /|     ` |");
}
	pspDebugScreenSetXY(6, adk + 7);
if (adk < 17) {
	printf(" | |       |");
}
	pspDebugScreenSetXY(6, adk + 8);
if (adk < 16) {
	printf(" |         |");
}
	pspDebugScreenSetXY(6, adk + 9);
if (adk < 15) {
	printf("  \\       / ");
}
	pspDebugScreenSetXY(6, adk + 10);
if (adk < 14) {
	printf("   |     |  ");
}
	pspDebugScreenSetXY(6, adk + 11);
if (adk < 13) {
	printf("            ");
}
	sceDisplayWaitVblankStart();
}
}

void big_intro() {
int adk;
int atk = 0;
int ask;

	for(adk= 35; adk>24; adk--) {
	pspDebugScreenSetTextColor(colorb);
	pspDebugScreenSetXY(0, atk - 10);
if (atk > 1) {
	printf("                                                              ");
}
	pspDebugScreenSetXY(0, atk - 9);
if (adk > 1) {
	printf("      ;                                                       ");
}
	pspDebugScreenSetXY(0, atk - 8);
if (atk > 1) {
	printf("      ;`-',                                                   ");
}
	pspDebugScreenSetXY(0, atk - 7);
if (atk > 1) {
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
}
	pspDebugScreenSetXY(0, atk - 6);
if (atk > 1) {
	printf("      ;`-';    /|    /|                                  ;`-';");
}
	pspDebugScreenSetXY(0, atk - 5);
if (atk > 1) {
	printf("      ;.-.;   //|/''/''/                                 ;.-.;");
}
	pspDebugScreenSetXY(0, atk - 4);
if (atk > 1) {
	printf("      ;`-';  //''||' ||                                  ;`-';");
}
	pspDebugScreenSetXY(0, atk - 3);
if (atk > 1) {
	printf("      ;.-.; |/   |/  |/                                  ;.-.;");
}
	pspDebugScreenSetXY(0, atk - 2);
if (atk > 1) {
	printf("      ;`-';                                              ;`-';");
}
	pspDebugScreenSetXY(0, atk - 1);
if (atk > 1) {
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
}
	pspDebugScreenSetXY(0, adk);
if (adk < 34) {
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
}
	pspDebugScreenSetXY(0, adk + 1);
if (adk < 34) {
	printf("      ;`-';                                              ;`-';");
}
	pspDebugScreenSetXY(0, adk + 2);
if (adk < 34) {
	printf("      ;.-.;                                              ;.-.;");
}
	pspDebugScreenSetXY(0, adk + 3);
if (adk < 34) {
	printf("      ;`-';                                              ;`-';");
}
	pspDebugScreenSetXY(0, adk + 4);
if (adk < 34) {
	printf("      ;.-.;                                              ;.-.;");
}
	pspDebugScreenSetXY(0, adk + 5);
if (adk < 34) {
	printf("      ;`-';                                              ;`-';");
}
	pspDebugScreenSetXY(0, adk + 6);
if (adk < 34) {
	printf("      ;.-.;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;.-.;");
}
	pspDebugScreenSetXY(0, adk + 7);
if (adk < 34) {
	printf("                                                              ");
}
	for(ask= 0; ask < 1; ask++) {
	sceDisplayWaitVblankStart();
	}
atk++;
if (atk == 12) {
atk = 11;
}

}
	pspDebugScreenSetTextColor(colord);
}

void print_config_menu() {
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(24, 13);
	printf("                   "); // covers up ok hand gesture artifact
	pspDebugScreenSetXY(24, 15);
	printf("                   ");
	pspDebugScreenSetXY(24, 17);
	printf("                   ");
	pspDebugScreenSetXY(24, 19);
	printf("                   ");
	pspDebugScreenSetXY(24, 12);
	printf("                   ");
	pspDebugScreenSetXY(24, 14);
if (moda == 0 ) {
	printf("NO KXPLOIT MODE ON ");
}
else {
	printf("NO KXPLOIT MODE OFF");
}
	pspDebugScreenSetXY(24, 16);
if (modb == 0 ) {
	printf("HIDE CORRUPT ICONS");
}
else {
	printf("SHOW CORRUPT ICONS");
}
	pspDebugScreenSetXY(23, 18);
if (modc == 0 ) {
	printf("SKIP COLDBOOT SCREEN");
}
else {
	printf("PLAY COLDBOOT SCREEN");
}
	pspDebugScreenSetXY(22, 20);
if (modd == 0 ) {
	printf("AUTOBOOT USER PROGRAM");
}
else {
	printf("  NO AUTOBOOT PROGRAM");
}
	sceDisplayWaitVblankStart();
}

void toggle_moda() {
	if (moda == 0) {
	moda = 1;
	config[172] = 0x66;// f
	config[173] = 0x61;// a
	config[174] = 0x6C;// l
	config[175] = 0x73;// s
	config[176] = 0x65;// e
	config[177] = 0x3B;// ;
	}
	else {
	moda = 0;
	config[172] = 0x74;// t
	config[173] = 0x72;// r
	config[174] = 0x75;// u
	config[175] = 0x65;// e
	config[176] = 0x3B;// ;
	config[177] = 0x20;//  
	}
}
void toggle_modb() {
	if (modb == 0) {
	modb = 1;
	config[194] = 0x66;// f
	config[195] = 0x61;// a
	config[196] = 0x6C;// l
	config[197] = 0x73;// s
	config[198] = 0x65;// e
	config[199] = 0x3B;// ;
	}
	else {
	modb = 0;
	config[194] = 0x74;// t
	config[195] = 0x72;// r
	config[196] = 0x75;// u
	config[197] = 0x65;// e
	config[198] = 0x3B;// ;
	config[199] = 0x20;// 
	}
}
void toggle_modc() {
	if (modc == 0) {
	modc = 1;
	config[213] = 0x66;// f
	config[214] = 0x61;// a
	config[215] = 0x6C;// l
	config[216] = 0x73;// s
	config[217] = 0x65;// e
	config[218] = 0x3B;// ;
	}
	else {
	modc = 0;
	config[213] = 0x74;// t
	config[214] = 0x72;// r
	config[215] = 0x75;// u
	config[216] = 0x65;// e
	config[217] = 0x3B;// ;
	config[218] = 0x20;// 
	}
}
void toggle_modd() {
	if (modd == 0) {
	modd = 1;
	config[221] = 0x23;// #
	}
	else {
	modd = 0;
	config[221] = 0x20;//  
	}
}

void increment_configmenu() {
configmenu ++;
if (configmenu == 5){
configmenu = 1;
}
}

void decrement_configmenu() {
configmenu --;
if (configmenu == 0){
configmenu = 4;
}
}

void more_configmenu_stuff() {
int fincnt;

if (configmenu == 1) {
	finger_a();
}
if (configmenu == 2) {
	finger_b();
}
if (configmenu == 3) {
	finger_c();
}
if (configmenu == 4) {
	finger_d();
}

print_config_menu();
for(fincnt=0; fincnt<9; fincnt++) {
sceDisplayWaitVblankStart();
}

}

void do_configmenu_stuff() {
if (configmenu == 1) {
toggle_moda();
}
if (configmenu == 2) {
toggle_modb();
}
if (configmenu == 3) {
toggle_modc();
}
if (configmenu == 4) {
toggle_modd();
}
}

void config_footer() {
	pspDebugScreenSetTextColor(colorb); // header or footer
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';   Move joypad up and down to navigate menu.  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;      Press 'X' to toggle selected option.    ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';    Press Triangle to save config.txt file.   ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;    Press Square to delete config.txt file.   ;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';      Press 'O' to exit the configurator.     ;`-';");
	pspDebugScreenSetTextColor(colord);
}

void editor_footer() {
	pspDebugScreenSetTextColor(colorb); // header or footer
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';  Move the cursor with joypad left and right. ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;   Change a letter with joypad up and down.   ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';  Press Triangle to use the displayed labels. ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.; Press Left Shoulder to Load labels from file.;.-.;");
	pspDebugScreenSetXY(0, 30);
	printf("      ;`-';  Press 'X' to reset labels, or 'O' to exit.  ;`-';");
	pspDebugScreenSetTextColor(colord);
}

void reset_ebuffer() {
ebuffer[0] = 0x53; // S
ebuffer[1] = 0x65; // e
ebuffer[2] = 0x74; // t
ebuffer[3] = 0x74; // t
ebuffer[4] = 0x69; // i
ebuffer[5] = 0x6E; // n
ebuffer[6] = 0x67; // g
ebuffer[7] = 0x73; // S

ebuffer[8] = 0x50; // P
ebuffer[9] = 0x68; // h
ebuffer[10] = 0x6F; // o
ebuffer[11] = 0x74; // t
ebuffer[12] = 0x6F; // o

ebuffer[13] = 0x4D; // M
ebuffer[14] = 0x75; // u
ebuffer[15] = 0x73; // s
ebuffer[16] = 0x69; // i
ebuffer[17] = 0x63; // c

ebuffer[18] = 0x56; // V
ebuffer[19] = 0x69; // i
ebuffer[20] = 0x64; // d
ebuffer[21] = 0x65; // e
ebuffer[22] = 0x6F; // o

ebuffer[23] = 0x47; // G
ebuffer[24] = 0x61; // a
ebuffer[25] = 0x6D; // m
ebuffer[26] = 0x65; // e

ebuffer[27] = 0x4E; // N
ebuffer[28] = 0x65; // e
ebuffer[29] = 0x74; // t
ebuffer[30] = 0x77; // w
ebuffer[31] = 0x6F; // o
ebuffer[32] = 0x72; // r
ebuffer[33] = 0x6B; // k
ebuffer[34] = 0x20; // _
ebuffer[35] = 0x55; // U
ebuffer[36] = 0x70; // p
ebuffer[37] = 0x64; // d
ebuffer[38] = 0x61; // a
ebuffer[39] = 0x74; // t
ebuffer[40] = 0x65; // e

ebuffer[41] = 0x55; // U
ebuffer[42] = 0x53; // S
ebuffer[43] = 0x42; // B
ebuffer[44] = 0x20; // _
ebuffer[45] = 0x43; // C
ebuffer[46] = 0x6F; // o
ebuffer[47] = 0x6E; // n
ebuffer[48] = 0x6E; // n
ebuffer[49] = 0x65; // e
ebuffer[50] = 0x63; // c
ebuffer[51] = 0x74; // t
ebuffer[52] = 0x69; // i
ebuffer[53] = 0x6F; // o
ebuffer[54] = 0x6E; // n

ebuffer[55] = 0x56; // V
ebuffer[56] = 0x69; // i
ebuffer[57] = 0x64; // d
ebuffer[58] = 0x65; // e
ebuffer[59] = 0x6F; // o
ebuffer[60] = 0x20; // _
ebuffer[61] = 0x53; // S
ebuffer[62] = 0x65; // e
ebuffer[63] = 0x74; // t
ebuffer[64] = 0x74; // t
ebuffer[65] = 0x69; // i
ebuffer[66] = 0x6E; // n
ebuffer[67] = 0x67; // g
ebuffer[68] = 0x73; // s

ebuffer[69] = 0x50; // P
ebuffer[70] = 0x68; // h
ebuffer[71] = 0x6F; // o
ebuffer[72] = 0x74; // t
ebuffer[73] = 0x6F; // o
ebuffer[74] = 0x20; // _
ebuffer[75] = 0x53; // S
ebuffer[76] = 0x65; // e
ebuffer[77] = 0x74; // t
ebuffer[78] = 0x74; // t
ebuffer[79] = 0x69; // i
ebuffer[80] = 0x6E; // n
ebuffer[81] = 0x67; // g
ebuffer[82] = 0x73; // s

ebuffer[83] = 0x53; // S
ebuffer[84] = 0x79; // y
ebuffer[85] = 0x73; // s
ebuffer[86] = 0x74; // t
ebuffer[87] = 0x65; // e
ebuffer[88] = 0x6D; // m
ebuffer[89] = 0x20; // _
ebuffer[90] = 0x53; // S
ebuffer[91] = 0x65; // e
ebuffer[92] = 0x74; // t
ebuffer[93] = 0x74; // t
ebuffer[94] = 0x69; // i
ebuffer[95] = 0x6E; // n
ebuffer[96] = 0x67; // g
ebuffer[97] = 0x73; // s

ebuffer[98] = 0x44; // D
ebuffer[99] = 0x61; // a
ebuffer[100] = 0x74; // t
ebuffer[101] = 0x65; // e
ebuffer[102] = 0x20; // _
ebuffer[103] = 0x26; // &
ebuffer[104] = 0x20; // _
ebuffer[105] = 0x54; // T
ebuffer[106] = 0x69; // i
ebuffer[107] = 0x6D; // m
ebuffer[108] = 0x65; // e
ebuffer[109] = 0x20; // _
ebuffer[110] = 0x53; // S
ebuffer[111] = 0x65; // e
ebuffer[112] = 0x74; // t
ebuffer[113] = 0x74; // t
ebuffer[114] = 0x69; // i
ebuffer[115] = 0x6E; // n
ebuffer[116] = 0x67; // g
ebuffer[117] = 0x73; // s (20)

ebuffer[118] = 0x50; // P
ebuffer[119] = 0x6F; // o
ebuffer[120] = 0x77; // w
ebuffer[121] = 0x65; // e
ebuffer[122] = 0x72; // r
ebuffer[123] = 0x20; // _
ebuffer[124] = 0x53; // S
ebuffer[125] = 0x61; // a
ebuffer[126] = 0x76; // v
ebuffer[127] = 0x65; // e
ebuffer[128] = 0x20; // _
ebuffer[129] = 0x53; // S
ebuffer[130] = 0x65; // e
ebuffer[131] = 0x74; // t
ebuffer[132] = 0x74; // t
ebuffer[133] = 0x69; // i
ebuffer[134] = 0x6E; // n
ebuffer[135] = 0x67; // g
ebuffer[136] = 0x73; // s (19)

ebuffer[137] = 0x53; // S
ebuffer[138] = 0x6F; // o
ebuffer[139] = 0x75; // u
ebuffer[140] = 0x6E; // n
ebuffer[141] = 0x64; // d
ebuffer[142] = 0x20; // _
ebuffer[143] = 0x53; // S
ebuffer[144] = 0x65; // e
ebuffer[145] = 0x74; // t
ebuffer[146] = 0x74; // t
ebuffer[147] = 0x69; // i
ebuffer[148] = 0x6E; // n
ebuffer[149] = 0x67; // g
ebuffer[150] = 0x73; // s (14)

ebuffer[151] = 0x53; // S
ebuffer[152] = 0x65; // e
ebuffer[153] = 0x63; // c
ebuffer[154] = 0x75; // u
ebuffer[155] = 0x72; // r
ebuffer[156] = 0x69; // i
ebuffer[157] = 0x74; // t
ebuffer[158] = 0x79; // y
ebuffer[159] = 0x20; // _
ebuffer[160] = 0x53; // S
ebuffer[161] = 0x65; // e
ebuffer[162] = 0x74; // t
ebuffer[163] = 0x74; // t
ebuffer[164] = 0x69; // i
ebuffer[165] = 0x6E; // n
ebuffer[166] = 0x67; // g
ebuffer[167] = 0x73; // s (17)

ebuffer[168] = 0x4E; // N
ebuffer[169] = 0x65; // e
ebuffer[170] = 0x74; // t
ebuffer[171] = 0x77; // w
ebuffer[172] = 0x6F; // o
ebuffer[173] = 0x72; // r
ebuffer[174] = 0x6B; // k
ebuffer[175] = 0x20; // _
ebuffer[176] = 0x53; // S
ebuffer[177] = 0x65; // e
ebuffer[178] = 0x74; // t
ebuffer[179] = 0x74; // t
ebuffer[180] = 0x69; // i
ebuffer[181] = 0x6E; // n
ebuffer[182] = 0x67; // g
ebuffer[183] = 0x73; // s (16)

ebuffer[184] = 0x53; // S
ebuffer[185] = 0x61; // a
ebuffer[186] = 0x76; // v
ebuffer[187] = 0x65; // e
ebuffer[188] = 0x64; // d
ebuffer[189] = 0x20; // _
ebuffer[190] = 0x44; // D
ebuffer[191] = 0x61; // a
ebuffer[192] = 0x74; // t
ebuffer[193] = 0x61; // a
ebuffer[194] = 0x20; // _
ebuffer[195] = 0x55; // U
ebuffer[196] = 0x74; // t
ebuffer[197] = 0x69; // i
ebuffer[198] = 0x6C; // l
ebuffer[199] = 0x69; // i
ebuffer[200] = 0x74; // t
ebuffer[201] = 0x79; // y (18)

ebuffer[202] = 0x47; // G
ebuffer[203] = 0x61; // a
ebuffer[204] = 0x6D; // m
ebuffer[205] = 0x65; // e
ebuffer[206] = 0x20; // _
ebuffer[207] = 0x53; // S
ebuffer[208] = 0x68; // h
ebuffer[209] = 0x61; // a
ebuffer[210] = 0x72; // r
ebuffer[211] = 0x69; // i
ebuffer[212] = 0x6E; // n
ebuffer[213] = 0x67; // g (12)

ebuffer[214] = 0x4D; // M
ebuffer[215] = 0x65; // e
ebuffer[216] = 0x6D; // m
ebuffer[217] = 0x6F; // o
ebuffer[218] = 0x72; // r
ebuffer[219] = 0x79; // y
ebuffer[220] = 0x20; // _
ebuffer[221] = 0x53; // S
ebuffer[222] = 0x74; // t
ebuffer[223] = 0x69; // i
ebuffer[224] = 0x63; // c
ebuffer[225] = 0x6B; // k (12)

ebuffer[226] = 0x46; // F
ebuffer[227] = 0x72; // r
ebuffer[228] = 0x65; // e
ebuffer[229] = 0x65; // e
ebuffer[230] = 0x20; // _
ebuffer[231] = 0x53; // S
ebuffer[232] = 0x70; // p
ebuffer[233] = 0x61; // a
ebuffer[234] = 0x63; // c
ebuffer[235] = 0x65; // e (10)

ebuffer[236] = 0x20; // RE
ebuffer[237] = 0x20; // S
ebuffer[238] = 0x20; // E
ebuffer[239] = 0x20; // R
ebuffer[240] = 0x20; // V
ebuffer[241] = 0x20; // E
ebuffer[242] = 0x20; // D
ebuffer[243] = 0x20; // _
ebuffer[244] = 0x20; // F
ebuffer[245] = 0x20; // O
ebuffer[246] = 0x20; // R
ebuffer[247] = 0x20; // _
ebuffer[248] = 0x20; // E
ebuffer[249] = 0x20; // X
ebuffer[250] = 0x20; // P
ebuffer[251] = 0x20; // A
ebuffer[252] = 0x20; // N
ebuffer[253] = 0x20; // S
ebuffer[254] = 0x20; // I
ebuffer[255] = 0x20; // O
ebuffer[256] = 0x20; // N
ebuffer[257] = 0x20; // _
ebuffer[258] = 0x20; // _
ebuffer[259] = 0x20; // _
ebuffer[260] = 0x20; // _
}

void reset_ebuffercomp() {
ebuffercomp[0] = 0x53; // S
ebuffercomp[1] = 0x65; // e
ebuffercomp[2] = 0x74; // t
ebuffercomp[3] = 0x74; // t
ebuffercomp[4] = 0x69; // i
ebuffercomp[5] = 0x6E; // n
ebuffercomp[6] = 0x67; // g
ebuffercomp[7] = 0x73; // S

ebuffercomp[8] = 0x50; // P
ebuffercomp[9] = 0x68; // h
ebuffercomp[10] = 0x6F; // o
ebuffercomp[11] = 0x74; // t
ebuffercomp[12] = 0x6F; // o

ebuffercomp[13] = 0x4D; // M
ebuffercomp[14] = 0x75; // u
ebuffercomp[15] = 0x73; // s
ebuffercomp[16] = 0x69; // i
ebuffercomp[17] = 0x63; // c

ebuffercomp[18] = 0x56; // V
ebuffercomp[19] = 0x69; // i
ebuffercomp[20] = 0x64; // d
ebuffercomp[21] = 0x65; // e
ebuffercomp[22] = 0x6F; // o

ebuffercomp[23] = 0x47; // G
ebuffercomp[24] = 0x61; // a
ebuffercomp[25] = 0x6D; // m
ebuffercomp[26] = 0x65; // e

ebuffercomp[27] = 0x4E; // N
ebuffercomp[28] = 0x65; // e
ebuffercomp[29] = 0x74; // t
ebuffercomp[30] = 0x77; // w
ebuffercomp[31] = 0x6F; // o
ebuffercomp[32] = 0x72; // r
ebuffercomp[33] = 0x6B; // k
ebuffercomp[34] = 0x20; // _
ebuffercomp[35] = 0x55; // U
ebuffercomp[36] = 0x70; // p
ebuffercomp[37] = 0x64; // d
ebuffercomp[38] = 0x61; // a
ebuffercomp[39] = 0x74; // t
ebuffercomp[40] = 0x65; // e

ebuffercomp[41] = 0x55; // U
ebuffercomp[42] = 0x53; // S
ebuffercomp[43] = 0x42; // B
ebuffercomp[44] = 0x20; // _
ebuffercomp[45] = 0x43; // C
ebuffercomp[46] = 0x6F; // o
ebuffercomp[47] = 0x6E; // n
ebuffercomp[48] = 0x6E; // n
ebuffercomp[49] = 0x65; // e
ebuffercomp[50] = 0x63; // c
ebuffercomp[51] = 0x74; // t
ebuffercomp[52] = 0x69; // i
ebuffercomp[53] = 0x6F; // o
ebuffercomp[54] = 0x6E; // n

ebuffercomp[55] = 0x56; // V
ebuffercomp[56] = 0x69; // i
ebuffercomp[57] = 0x64; // d
ebuffercomp[58] = 0x65; // e
ebuffercomp[59] = 0x6F; // o
ebuffercomp[60] = 0x20; // _
ebuffercomp[61] = 0x53; // S
ebuffercomp[62] = 0x65; // e
ebuffercomp[63] = 0x74; // t
ebuffercomp[64] = 0x74; // t
ebuffercomp[65] = 0x69; // i
ebuffercomp[66] = 0x6E; // n
ebuffercomp[67] = 0x67; // g
ebuffercomp[68] = 0x73; // s

ebuffercomp[69] = 0x50; // P
ebuffercomp[70] = 0x68; // h
ebuffercomp[71] = 0x6F; // o
ebuffercomp[72] = 0x74; // t
ebuffercomp[73] = 0x6F; // o
ebuffercomp[74] = 0x20; // _
ebuffercomp[75] = 0x53; // S
ebuffercomp[76] = 0x65; // e
ebuffercomp[77] = 0x74; // t
ebuffercomp[78] = 0x74; // t
ebuffercomp[79] = 0x69; // i
ebuffercomp[80] = 0x6E; // n
ebuffercomp[81] = 0x67; // g
ebuffercomp[82] = 0x73; // s

ebuffercomp[83] = 0x53; // S
ebuffercomp[84] = 0x79; // y
ebuffercomp[85] = 0x73; // s
ebuffercomp[86] = 0x74; // t
ebuffercomp[87] = 0x65; // e
ebuffercomp[88] = 0x6D; // m
ebuffercomp[89] = 0x20; // _
ebuffercomp[90] = 0x53; // S
ebuffercomp[91] = 0x65; // e
ebuffercomp[92] = 0x74; // t
ebuffercomp[93] = 0x74; // t
ebuffercomp[94] = 0x69; // i
ebuffercomp[95] = 0x6E; // n
ebuffercomp[96] = 0x67; // g
ebuffercomp[97] = 0x73; // s

ebuffercomp[98] = 0x44; // D
ebuffercomp[99] = 0x61; // a
ebuffercomp[100] = 0x74; // t
ebuffercomp[101] = 0x65; // e
ebuffercomp[102] = 0x20; // _
ebuffercomp[103] = 0x26; // &
ebuffercomp[104] = 0x20; // _
ebuffercomp[105] = 0x54; // T
ebuffercomp[106] = 0x69; // i
ebuffercomp[107] = 0x6D; // m
ebuffercomp[108] = 0x65; // e
ebuffercomp[109] = 0x20; // _
ebuffercomp[110] = 0x53; // S
ebuffercomp[111] = 0x65; // e
ebuffercomp[112] = 0x74; // t
ebuffercomp[113] = 0x74; // t
ebuffercomp[114] = 0x69; // i
ebuffercomp[115] = 0x6E; // n
ebuffercomp[116] = 0x67; // g
ebuffercomp[117] = 0x73; // s (20)

ebuffercomp[118] = 0x50; // P
ebuffercomp[119] = 0x6F; // o
ebuffercomp[120] = 0x77; // w
ebuffercomp[121] = 0x65; // e
ebuffercomp[122] = 0x72; // r
ebuffercomp[123] = 0x20; // _
ebuffercomp[124] = 0x53; // S
ebuffercomp[125] = 0x61; // a
ebuffercomp[126] = 0x76; // v
ebuffercomp[127] = 0x65; // e
ebuffercomp[128] = 0x20; // _
ebuffercomp[129] = 0x53; // S
ebuffercomp[130] = 0x65; // e
ebuffercomp[131] = 0x74; // t
ebuffercomp[132] = 0x74; // t
ebuffercomp[133] = 0x69; // i
ebuffercomp[134] = 0x6E; // n
ebuffercomp[135] = 0x67; // g
ebuffercomp[136] = 0x73; // s (19)

ebuffercomp[137] = 0x53; // S
ebuffercomp[138] = 0x6F; // o
ebuffercomp[139] = 0x75; // u
ebuffercomp[140] = 0x6E; // n
ebuffercomp[141] = 0x64; // d
ebuffercomp[142] = 0x20; // _
ebuffercomp[143] = 0x53; // S
ebuffercomp[144] = 0x65; // e
ebuffercomp[145] = 0x74; // t
ebuffercomp[146] = 0x74; // t
ebuffercomp[147] = 0x69; // i
ebuffercomp[148] = 0x6E; // n
ebuffercomp[149] = 0x67; // g
ebuffercomp[150] = 0x73; // s (14)

ebuffercomp[151] = 0x53; // S
ebuffercomp[152] = 0x65; // e
ebuffercomp[153] = 0x63; // c
ebuffercomp[154] = 0x75; // u
ebuffercomp[155] = 0x72; // r
ebuffercomp[156] = 0x69; // i
ebuffercomp[157] = 0x74; // t
ebuffercomp[158] = 0x79; // y
ebuffercomp[159] = 0x20; // _
ebuffercomp[160] = 0x53; // S
ebuffercomp[161] = 0x65; // e
ebuffercomp[162] = 0x74; // t
ebuffercomp[163] = 0x74; // t
ebuffercomp[164] = 0x69; // i
ebuffercomp[165] = 0x6E; // n
ebuffercomp[166] = 0x67; // g
ebuffercomp[167] = 0x73; // s (17)

ebuffercomp[168] = 0x4E; // N
ebuffercomp[169] = 0x65; // e
ebuffercomp[170] = 0x74; // t
ebuffercomp[171] = 0x77; // w
ebuffercomp[172] = 0x6F; // o
ebuffercomp[173] = 0x72; // r
ebuffercomp[174] = 0x6B; // k
ebuffercomp[175] = 0x20; // _
ebuffercomp[176] = 0x53; // S
ebuffercomp[177] = 0x65; // e
ebuffercomp[178] = 0x74; // t
ebuffercomp[179] = 0x74; // t
ebuffercomp[180] = 0x69; // i
ebuffercomp[181] = 0x6E; // n
ebuffercomp[182] = 0x67; // g
ebuffercomp[183] = 0x73; // s (16)

ebuffercomp[184] = 0x53; // S
ebuffercomp[185] = 0x61; // a
ebuffercomp[186] = 0x76; // v
ebuffercomp[187] = 0x65; // e
ebuffercomp[188] = 0x64; // d
ebuffercomp[189] = 0x20; // _
ebuffercomp[190] = 0x44; // D
ebuffercomp[191] = 0x61; // a
ebuffercomp[192] = 0x74; // t
ebuffercomp[193] = 0x61; // a
ebuffercomp[194] = 0x20; // _
ebuffercomp[195] = 0x55; // U
ebuffercomp[196] = 0x74; // t
ebuffercomp[197] = 0x69; // i
ebuffercomp[198] = 0x6C; // l
ebuffercomp[199] = 0x69; // i
ebuffercomp[200] = 0x74; // t
ebuffercomp[201] = 0x79; // y (18)

ebuffercomp[202] = 0x47; // G
ebuffercomp[203] = 0x61; // a
ebuffercomp[204] = 0x6D; // m
ebuffercomp[205] = 0x65; // e
ebuffercomp[206] = 0x20; // _
ebuffercomp[207] = 0x53; // S
ebuffercomp[208] = 0x68; // h
ebuffercomp[209] = 0x61; // a
ebuffercomp[210] = 0x72; // r
ebuffercomp[211] = 0x69; // i
ebuffercomp[212] = 0x6E; // n
ebuffercomp[213] = 0x67; // g (12)

ebuffercomp[214] = 0x4D; // M
ebuffercomp[215] = 0x65; // e
ebuffercomp[216] = 0x6D; // m
ebuffercomp[217] = 0x6F; // o
ebuffercomp[218] = 0x72; // r
ebuffercomp[219] = 0x79; // y
ebuffercomp[220] = 0x20; // _
ebuffercomp[221] = 0x53; // S
ebuffercomp[222] = 0x74; // t
ebuffercomp[223] = 0x69; // i
ebuffercomp[224] = 0x63; // c
ebuffercomp[225] = 0x6B; // k (12)

ebuffercomp[226] = 0x46; // F
ebuffercomp[227] = 0x72; // r
ebuffercomp[228] = 0x65; // e
ebuffercomp[229] = 0x65; // e
ebuffercomp[230] = 0x20; // _
ebuffercomp[231] = 0x53; // S
ebuffercomp[232] = 0x70; // p
ebuffercomp[233] = 0x61; // a
ebuffercomp[234] = 0x63; // c
ebuffercomp[235] = 0x65; // e (10)

ebuffercomp[236] = 0x20; // RE
ebuffercomp[237] = 0x20; // S
ebuffercomp[238] = 0x20; // E
ebuffercomp[239] = 0x20; // R
ebuffercomp[240] = 0x20; // V
ebuffercomp[241] = 0x20; // E
ebuffercomp[242] = 0x20; // D
ebuffercomp[243] = 0x20; // _
ebuffercomp[244] = 0x20; // F
ebuffercomp[245] = 0x20; // O
ebuffercomp[246] = 0x20; // R
ebuffercomp[247] = 0x20; // _
ebuffercomp[248] = 0x20; // E
ebuffercomp[249] = 0x20; // X
ebuffercomp[250] = 0x20; // P
ebuffercomp[251] = 0x20; // A
ebuffercomp[252] = 0x20; // N
ebuffercomp[253] = 0x20; // S
ebuffercomp[254] = 0x20; // I
ebuffercomp[255] = 0x20; // O
ebuffercomp[256] = 0x20; // N
ebuffercomp[257] = 0x20; // _
ebuffercomp[258] = 0x20; // _
ebuffercomp[259] = 0x20; // _
ebuffercomp[260] = 0x20; // _
}

void set_ebufferxor() {
ebufferxor[0] = 0x2A; // S
ebufferxor[1] = 0x94; // e
ebufferxor[2] = 0xDC; // t
ebufferxor[3] = 0x4F; // t
ebufferxor[4] = 0xD1; // i
ebufferxor[5] = 0x1F; // n
ebufferxor[6] = 0xCB; // g
ebufferxor[7] = 0x62; // S

ebufferxor[8] = 0xCE; // P
ebufferxor[9] = 0x01; // h
ebufferxor[10] = 0x6C; // o
ebufferxor[11] = 0xE9; // t
ebufferxor[12] = 0x7B; // o

ebufferxor[13] = 0x4E; // M
ebufferxor[14] = 0xE2; // u
ebufferxor[15] = 0x6E; // s
ebufferxor[16] = 0xEE; // i
ebufferxor[17] = 0x8C; // c

ebufferxor[18] = 0x25; // V
ebufferxor[19] = 0x0D; // i
ebufferxor[20] = 0x38; // d
ebufferxor[21] = 0x3C; // e
ebufferxor[22] = 0x6A; // o

ebufferxor[23] = 0x8B; // G
ebufferxor[24] = 0x32; // a
ebufferxor[25] = 0x6C; // m
ebufferxor[26] = 0x55; // e

ebufferxor[27] = 0xEB; // N
ebufferxor[28] = 0x04; // e
ebufferxor[29] = 0xF1; // t
ebufferxor[30] = 0x44; // w
ebufferxor[31] = 0x95; // o
ebufferxor[32] = 0xAE; // r
ebufferxor[33] = 0x08; // k
ebufferxor[34] = 0x75; // _
ebufferxor[35] = 0xB8; // U
ebufferxor[36] = 0x68; // p
ebufferxor[37] = 0x3D; // d
ebufferxor[38] = 0xA3; // a
ebufferxor[39] = 0xF4; // t
ebufferxor[40] = 0x39; // e

ebufferxor[41] = 0xC3; // U
ebufferxor[42] = 0x99; // S
ebufferxor[43] = 0x20; // B
ebufferxor[44] = 0xE4; // _
ebufferxor[45] = 0x8D; // C
ebufferxor[46] = 0xDD; // o
ebufferxor[47] = 0x1F; // n
ebufferxor[48] = 0x37; // n
ebufferxor[49] = 0x7B; // e
ebufferxor[50] = 0xCD; // c
ebufferxor[51] = 0x6E; // t
ebufferxor[52] = 0x68; // i
ebufferxor[53] = 0x22; // o
ebufferxor[54] = 0xF6; // n

ebufferxor[55] = 0xB6; // V
ebufferxor[56] = 0xC1; // i
ebufferxor[57] = 0xDD; // d
ebufferxor[58] = 0x0F; // e
ebufferxor[59] = 0xC9; // o
ebufferxor[60] = 0x24; // _
ebufferxor[61] = 0xA5; // S
ebufferxor[62] = 0x71; // e
ebufferxor[63] = 0xD8; // t
ebufferxor[64] = 0x72; // t
ebufferxor[65] = 0x95; // i
ebufferxor[66] = 0x21; // n
ebufferxor[67] = 0x3F; // g
ebufferxor[68] = 0xD3; // s

ebufferxor[69] = 0xDB; // P
ebufferxor[70] = 0x38; // h
ebufferxor[71] = 0x6F; // o
ebufferxor[72] = 0x44; // t
ebufferxor[73] = 0x2F; // o
ebufferxor[74] = 0x2D; // _
ebufferxor[75] = 0xC7; // S
ebufferxor[76] = 0x55; // e
ebufferxor[77] = 0xD7; // t
ebufferxor[78] = 0x42; // t
ebufferxor[79] = 0x69; // i
ebufferxor[80] = 0xCC; // n
ebufferxor[81] = 0x12; // g
ebufferxor[82] = 0x51; // s

ebufferxor[83] = 0x81; // S
ebufferxor[84] = 0x39; // y
ebufferxor[85] = 0x3C; // s
ebufferxor[86] = 0x03; // t
ebufferxor[87] = 0x3B; // e
ebufferxor[88] = 0xB0; // m
ebufferxor[89] = 0x95; // _
ebufferxor[90] = 0x97; // S
ebufferxor[91] = 0x81; // e
ebufferxor[92] = 0x70; // t
ebufferxor[93] = 0xF4; // t
ebufferxor[94] = 0xA3; // i
ebufferxor[95] = 0x71; // n
ebufferxor[96] = 0xD3; // g
ebufferxor[97] = 0x83; // s

ebufferxor[98] = 0xA1; // D
ebufferxor[99] = 0xB1; // a
ebufferxor[100] = 0xC1; // t
ebufferxor[101] = 0xC2; // e
ebufferxor[102] = 0xC3; // _
ebufferxor[103] = 0xC4; // &
ebufferxor[104] = 0xC5; // _
ebufferxor[105] = 0xC6; // T
ebufferxor[106] = 0xC7; // i
ebufferxor[107] = 0xC8; // m
ebufferxor[108] = 0xC9; // e
ebufferxor[109] = 0x2A; // _
ebufferxor[110] = 0x2B; // S
ebufferxor[111] = 0x2C; // e
ebufferxor[112] = 0x2D; // t
ebufferxor[113] = 0x2E; // t
ebufferxor[114] = 0x2F; // i
ebufferxor[115] = 0x2F; // n
ebufferxor[116] = 0x3F; // g
ebufferxor[117] = 0x5F; // s (20)

ebufferxor[118] = 0x2D; // P
ebufferxor[119] = 0x77; // o
ebufferxor[120] = 0xAE; // w
ebufferxor[121] = 0xD7; // e
ebufferxor[122] = 0xB8; // r
ebufferxor[123] = 0x7C; // _
ebufferxor[124] = 0xB3; // S
ebufferxor[125] = 0xD8; // a
ebufferxor[126] = 0x71; // v
ebufferxor[127] = 0x95; // e
ebufferxor[128] = 0x0A; // _
ebufferxor[129] = 0xFD; // S
ebufferxor[130] = 0x2C; // e
ebufferxor[131] = 0xDC; // t
ebufferxor[132] = 0x43; // t
ebufferxor[133] = 0x20; // i
ebufferxor[134] = 0x20; // n
ebufferxor[135] = 0x20; // g
ebufferxor[136] = 0x20; // s (19)

ebufferxor[137] = 0x20; // S
ebufferxor[138] = 0x20; // o
ebufferxor[139] = 0x11; // u
ebufferxor[140] = 0x22; // n
ebufferxor[141] = 0x33; // d
ebufferxor[142] = 0x44; // _
ebufferxor[143] = 0x55; // S
ebufferxor[144] = 0x66; // e
ebufferxor[145] = 0x77; // t
ebufferxor[146] = 0x88; // t
ebufferxor[147] = 0x99; // i
ebufferxor[148] = 0xAA; // n
ebufferxor[149] = 0xBB; // g
ebufferxor[150] = 0xDD; // s (14)

ebufferxor[151] = 0xF3; // S
ebufferxor[152] = 0x54; // e
ebufferxor[153] = 0x53; // c
ebufferxor[154] = 0xD5; // u
ebufferxor[155] = 0x20; // r
ebufferxor[156] = 0x20; // i
ebufferxor[157] = 0x20; // t
ebufferxor[158] = 0x20; // y
ebufferxor[159] = 0x25; // _
ebufferxor[160] = 0x34; // S
ebufferxor[161] = 0x20; // e
ebufferxor[162] = 0x20; // t
ebufferxor[163] = 0x20; // t
ebufferxor[164] = 0x20; // i
ebufferxor[165] = 0x20; // n
ebufferxor[166] = 0x54; // g
ebufferxor[167] = 0x20; // s (17)

ebufferxor[168] = 0x20; // N
ebufferxor[169] = 0x20; // e
ebufferxor[170] = 0x54; // t
ebufferxor[171] = 0x68; // w
ebufferxor[172] = 0x65; // o
ebufferxor[173] = 0x79; // r
ebufferxor[174] = 0x27; // k
ebufferxor[175] = 0x6C; // _
ebufferxor[176] = 0x6C; // S
ebufferxor[177] = 0x20; // e
ebufferxor[178] = 0x6E; // t
ebufferxor[179] = 0x65; // t
ebufferxor[180] = 0x76; // i
ebufferxor[181] = 0x65; // n
ebufferxor[182] = 0x72; // g
ebufferxor[183] = 0x20; // s (16)

ebufferxor[184] = 0x67; // S
ebufferxor[185] = 0x65; // a
ebufferxor[186] = 0x74; // v
ebufferxor[187] = 0x20; // e
ebufferxor[188] = 0x74; // d
ebufferxor[189] = 0x68; // _
ebufferxor[190] = 0x69; // D
ebufferxor[191] = 0x73; // a
ebufferxor[192] = 0x21; // t
ebufferxor[193] = 0x21; // a
ebufferxor[194] = 0x21; // _
ebufferxor[195] = 0x20; // U
ebufferxor[196] = 0x20; // t
ebufferxor[197] = 0x20; // i
ebufferxor[198] = 0x20; // l
ebufferxor[199] = 0xF3; // i
ebufferxor[200] = 0x9A; // t
ebufferxor[201] = 0x5F; // y (18)

ebufferxor[202] = 0x3F; // G
ebufferxor[203] = 0x33; // a
ebufferxor[204] = 0x83; // m
ebufferxor[205] = 0x2D; // e
ebufferxor[206] = 0xC9; // _
ebufferxor[207] = 0x43; // S
ebufferxor[208] = 0xF8; // h
ebufferxor[209] = 0xA1; // a
ebufferxor[210] = 0xDB; // r
ebufferxor[211] = 0x69; // i
ebufferxor[212] = 0x3E; // n
ebufferxor[213] = 0xB5; // g (12)

ebufferxor[214] = 0xA6; // M
ebufferxor[215] = 0xBB; // e
ebufferxor[216] = 0x24; // m
ebufferxor[217] = 0x6E; // o
ebufferxor[218] = 0x72; // r
ebufferxor[219] = 0x23; // y
ebufferxor[220] = 0xFD; // _
ebufferxor[221] = 0x54; // S
ebufferxor[222] = 0x7A; // t
ebufferxor[223] = 0x61; // i
ebufferxor[224] = 0xF0; // c
ebufferxor[225] = 0x3C; // k (12)

ebufferxor[226] = 0x42; // F
ebufferxor[227] = 0x70; // r
ebufferxor[228] = 0xD3; // e
ebufferxor[229] = 0x66; // e
ebufferxor[230] = 0x9C; // _
ebufferxor[231] = 0xB4; // S
ebufferxor[232] = 0xB5; // p
ebufferxor[233] = 0x22; // a
ebufferxor[234] = 0x3E; // c
ebufferxor[235] = 0xDF; // e (10)

ebufferxor[236] = 0xCA; // RE
ebufferxor[237] = 0xFE; // S
ebufferxor[238] = 0x20; // E
ebufferxor[239] = 0xBA; // R
ebufferxor[240] = 0xDC; // V
ebufferxor[241] = 0xAF; // E
ebufferxor[242] = 0xE0; // D
ebufferxor[243] = 0x20; // _
ebufferxor[244] = 0x32; // F
ebufferxor[245] = 0x79; // O
ebufferxor[246] = 0x23; // R
ebufferxor[247] = 0x34; // _
ebufferxor[248] = 0xD5; // E
ebufferxor[249] = 0x23; // X
ebufferxor[250] = 0x69; // P
ebufferxor[251] = 0x73; // A
ebufferxor[252] = 0xEF; // N
ebufferxor[253] = 0x59; // S
ebufferxor[254] = 0x11; // I
ebufferxor[255] = 0x60; // O
ebufferxor[256] = 0x23; // N
ebufferxor[257] = 0xDD; // _
ebufferxor[258] = 0x5A; // _
ebufferxor[259] = 0x20; // _
ebufferxor[260] = 0x20; // _
}

void invert_cursor() {

if (cursor != editcnt) {
pspDebugScreenSetBackColor(0x00000000);
pspDebugScreenSetTextColor(colord);
}
else {
if (freespace == 1) {
if (colcur < 5236) {
pspDebugScreenSetBackColor(0x00000000);
pspDebugScreenSetTextColor(0x00EEEEEE);
}

if (colcur < 2607) {
pspDebugScreenSetBackColor(0x00EEEEEE);
pspDebugScreenSetTextColor(0x00000000);
}

if (colcur > 5235) {
pspDebugScreenSetBackColor(colord);
pspDebugScreenSetTextColor(0x00000000);
}
}

if (freespace == 0) {
if (colcur < 5226) {
pspDebugScreenSetBackColor(0x00000000);
pspDebugScreenSetTextColor(0x00EEEEEE);
}

if (colcur < 2607) {
pspDebugScreenSetBackColor(0x00EEEEEE);
pspDebugScreenSetTextColor(0x00000000);
}

if (colcur > 5225) {
pspDebugScreenSetBackColor(colord);
pspDebugScreenSetTextColor(0x00000000);
}
}
}

if (cursor != editcnt) {
if (ebuffer[editcnt] != ebuffercomp[editcnt]) {
pspDebugScreenSetBackColor(colord);
pspDebugScreenSetTextColor(0x00000000);
}
}


colcur++;

if (freespace == 1) {
if (colcur > 5240) {  // no lower than 5216
colcur = 0;
}
}
if (freespace == 0) {
if (colcur > 5230) {  // no lower than 5216
colcur = 0;
}
}
}

void print_editor_menu() {
	int acnt;
	int xpos;
	int ypos;
	editcnt = 0;

xpos = 11;
ypos = 12;
for(acnt=0; acnt<8; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 24;
ypos = 12;
for(acnt=0; acnt<5; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 33;
ypos = 12;
for(acnt=0; acnt<5; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 43;
ypos = 12;
for(acnt=0; acnt<5; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 53;
ypos = 12;
for(acnt=0; acnt<4; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 14;
for(acnt=0; acnt<14; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 15;
for(acnt=0; acnt<14; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 16;
for(acnt=0; acnt<14; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 17;
for(acnt=0; acnt<14; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 18;
for(acnt=0; acnt<15; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 19;
for(acnt=0; acnt<20; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 20;
for(acnt=0; acnt<19; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 21;
for(acnt=0; acnt<14; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 22;
for(acnt=0; acnt<17; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 11;
ypos = 23;
for(acnt=0; acnt<16; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 39;
ypos = 14;
for(acnt=0; acnt<18; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 45;
ypos = 15;
for(acnt=0; acnt<12; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

xpos = 45;
ypos = 17;
for(acnt=0; acnt<12; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}

if (freespace == 1) {
xpos = 47;
ypos = 18;
for(acnt=0; acnt<10; acnt++) {
invert_cursor();
	pspDebugScreenSetXY(xpos, ypos);
	printf("%c",ebuffer[editcnt]);
	xpos++;
	editcnt++;
}
}


	sceDisplayWaitVblankStart();
}

void print_eq() {
get_eq_values();
widtha = 24 - widtha;
widthb = 24 - widthb;
widthc = 24 - widthc;
widthd = 24 - widthd;
widthe = 24 - widthe;
widthf = 24 - widthf;
widthg = 24 - widthg;
widthh = 24 - widthh;
widthi = 24 - widthi;
widthj = 24 - widthj;
widthk = 24 - widthk;
widthl = 24 - widthl;
widthm = 24 - widthm;
widthn = 24 - widthn;
widtho = 24 - widtho;
widthp = 24 - widthp;
widthq = 24 - widthq;
widthr = 24 - widthr;
widths = 24 - widths;
widtht = 24 - widtht;
widthu = 24 - widthu;
widthv = 24 - widthv;
widthw = 24 - widthw;
widthx = 24 - widthx;

if (widtha < 10) {
widtha = 10;
}
if (widthb < 10) {
widthb = 10;
}
if (widthc < 10) {
widthc = 10;
}
if (widthd < 10) {
widthd = 10;
}
if (widthe < 10) {
widthe = 10;
}
if (widthf < 10) {
widthf = 10;
}
if (widthg < 10) {
widthg = 10;
}
if (widthh < 10) {
widthh = 10;
}
if (widthi < 10) {
widthi = 10;
}
if (widthj < 10) {
widthj = 10;
}
if (widthk < 10) {
widthk = 10;
}
if (widthl < 10) {
widthl = 10;
}
if (widthm < 10) {
widthm = 10;
}
if (widthn < 10) {
widthn = 10;
}
if (widtho < 10) {
widtho = 10;
}
if (widthp < 10) {
widthp = 10;
}
if (widthq < 10) {
widthq = 10;
}
if (widthr < 10) {
widthr = 10;
}
if (widths < 10) {
widths = 10;
}
if (widtht < 10) {
widtht = 10;
}
if (widthu < 10) {
widthu = 10;
}
if (widthv < 10) {
widthv = 10;
}
if (widthw < 10) {
widthw = 10;
}
if (widthx < 10) {
widthx = 10;
}

if (mcnt > 85) {
int i;
for(i=0; i< mcnt - 86; i++) {
scroll_freq();
}
}

xo = 10;
yo = 24;
xi = 10;
yi = widtha;
draw_bar();
xo = 12;
yo = 24;
xi = 12;
yi = widthb;
draw_bar();
xo = 14;
yo = 24;
xi = 14;
yi = widthc;
draw_bar();
xo = 16;
yo = 24;
xi = 16;
yi = widthd;
draw_bar();
xo = 18;
yo = 24;
xi = 18;
yi = widthe;
draw_bar();
xo = 20;
yo = 24;
xi = 20;
yi = widthf;
draw_bar();
xo = 22;
yo = 24;
xi = 22;
yi = widthg;
draw_bar();
xo = 24;
yo = 24;
xi = 24;
yi = widthh;
draw_bar();
xo = 26;
yo = 24;
xi = 26;
yi = widthi;
draw_bar();
xo = 28;
yo = 24;
xi = 28;
yi = widthj;
draw_bar();
xo = 30;
yo = 24;
xi = 30;
yi = widthk;
draw_bar();
xo = 32;
yo = 24;
xi = 32;
yi = widthl;
draw_bar();
xo = 34;
yo = 24;
xi = 34;
yi = widthm;
draw_bar();
xo = 36;
yo = 24;
xi = 36;
yi = widthn;
draw_bar();
xo = 38;
yo = 24;
xi = 38;
yi = widtho;
draw_bar();
xo = 40;
yo = 24;
xi = 40;
yi = widthp;
draw_bar();
xo = 42;
yo = 24;
xi = 42;
yi = widthq;
draw_bar();
xo = 44;
yo = 24;
xi = 44;
yi = widthr;
draw_bar();
xo = 46;
yo = 24;
xi = 46;
yi = widths;
draw_bar();
xo = 48;
yo = 24;
xi = 48;
yi = widtht;
draw_bar();
xo = 50;
yo = 24;
xi = 50;
yi = widthu;
draw_bar();
xo = 52;
yo = 24;
xi = 52;
yi = widthv;
draw_bar();
xo = 54;
yo = 24;
xi = 54;
yi = widthw;
draw_bar();
xo = 56;
yo = 24;
xi = 56;
yi = widthx;
draw_bar();
}

void scroll_freq() {
twidth = widtha;
widtha = widthb;
widthb = widthc;
widthc = widthd;
widthd = widthe;
widthe = widthf;
widthf = widthg;
widthg = widthh;
widthh = widthi;
widthi = widthj;
widthj = widthk;
widthk = widthl;
widthl = widthm;
widthm = widthn;
widthn = widtho;
widtho = widthp;
widthp = widthq;
widthq = widthr;
widthr = widths;
widths = widtht;
widtht = widthu;
widthu = widthv;
widthv = widthw;
widthw = widthx;
widthx = twidth;
}

void editor_save() {
int ost;
int ofn;
int est;

est = 0;
ost = 16280;
for(ofn=0; ofn<8; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 8;
ost = 16300;
for(ofn=0; ofn<5; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 13;
ost = 16312;
for(ofn=0; ofn<5; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 18;
ost = 16324;
for(ofn=0; ofn<5; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 23;
ost = 16336;
for(ofn=0; ofn<4; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 27;
ost = 16904;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 41;
ost = 16632;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 55;
ost = 16348;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 69;
ost = 16380;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 83;
ost = 16412;
for(ofn=0; ofn<15; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 98;
ost = 16588;
for(ofn=0; ofn<20; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 118;
ost = 16480;
for(ofn=0; ofn<19; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 137;
ost = 16520;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 151;
ost = 16444;
for(ofn=0; ofn<17; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 168;
ost = 16552;
for(ofn=0; ofn<16; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 184;
ost = 16664;
for(ofn=0; ofn<18; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 202;
ost = 16936;
for(ofn=0; ofn<12; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 214;
ost = 14700;
for(ofn=0; ofn<12; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

if (freespace == 1) {		// Easter Egg
est = 226;			// Free Space
ost = 14916;
for(ofn=0; ofn<10; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}
}

}

void kws_editor_save() {
int ost;
int ofn;
int est;

est = 0;
ost = 18564;
for(ofn=0; ofn<8; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 8;
ost = 18584;
for(ofn=0; ofn<5; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 13;
ost = 18596;
for(ofn=0; ofn<5; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 18;
ost = 18608;
for(ofn=0; ofn<5; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 23;
ost = 18620;
for(ofn=0; ofn<4; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 168;			// Network label fix for newer firmware
ost = 18632;			// copies Network string from Network_Update
for(ofn=0; ofn<7; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 27;
ost = 19204;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 41;
ost = 18932;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 55;
ost = 18648;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 69;
ost = 18680;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 83;
ost = 18712;
for(ofn=0; ofn<15; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 98;
ost = 18888;
for(ofn=0; ofn<20; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 118;
ost = 18780;
for(ofn=0; ofn<19; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 137;
ost = 18820;
for(ofn=0; ofn<14; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 151;
ost = 18744;
for(ofn=0; ofn<17; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 168;
ost = 18852;
for(ofn=0; ofn<16; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 184;
ost = 18964;
for(ofn=0; ofn<18; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 202;
ost = 19236;
for(ofn=0; ofn<12; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

est = 214;
ost = 16972;
for(ofn=0; ofn<12; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}

if (freespace == 1) {		// Easter Egg
est = 226;			// Free Space
ost = 17188;
for(ofn=0; ofn<10; ofn++) {
opening[ost] = ebuffer[est];
est++;
ost = ost + 2;
}
}

}

void editor_read() {
int ost;
int ofn;
int est;

est = 0;
ost = 16280;
for(ofn=0; ofn<8; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 8;
ost = 16300;
for(ofn=0; ofn<5; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 13;
ost = 16312;
for(ofn=0; ofn<5; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 18;
ost = 16324;
for(ofn=0; ofn<5; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 23;
ost = 16336;
for(ofn=0; ofn<4; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 27;
ost = 16904;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 41;
ost = 16632;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 55;
ost = 16348;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 69;
ost = 16380;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 83;
ost = 16412;
for(ofn=0; ofn<15; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 98;
ost = 16588;
for(ofn=0; ofn<20; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 118;
ost = 16480;
for(ofn=0; ofn<19; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 137;
ost = 16520;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 151;
ost = 16444;
for(ofn=0; ofn<17; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 168;
ost = 16552;
for(ofn=0; ofn<16; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 184;
ost = 16664;
for(ofn=0; ofn<18; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 202;
ost = 16936;
for(ofn=0; ofn<12; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 214;
ost = 14700;
for(ofn=0; ofn<12; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

if (freespace == 1) {		// Easter Egg
est = 226;			// Free Space
ost = 14916;
for(ofn=0; ofn<10; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}
}

}

void kws_editor_read() {
int ost;
int ofn;
int est;

est = 0;
ost = 18564;
for(ofn=0; ofn<8; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 8;
ost = 18584;
for(ofn=0; ofn<5; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 13;
ost = 18596;
for(ofn=0; ofn<5; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 18;
ost = 18608;
for(ofn=0; ofn<5; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 23;
ost = 18620;
for(ofn=0; ofn<4; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 27;
ost = 19204;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 41;
ost = 18932;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 55;
ost = 18648;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 69;
ost = 18680;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 83;
ost = 18712;
for(ofn=0; ofn<15; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 98;
ost = 18888;
for(ofn=0; ofn<20; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 118;
ost = 18780;
for(ofn=0; ofn<19; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 137;
ost = 18820;
for(ofn=0; ofn<14; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 151;
ost = 18744;
for(ofn=0; ofn<17; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 168;
ost = 18852;
for(ofn=0; ofn<16; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 184;
ost = 18964;
for(ofn=0; ofn<18; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 202;
ost = 19236;
for(ofn=0; ofn<12; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

est = 214;
ost = 16972;
for(ofn=0; ofn<12; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}

if (freespace == 1) {		// Easter Egg
est = 226;			// Free Space
ost = 17188;
for(ofn=0; ofn<10; ofn++) {
ebuffer[est] = opening[ost];
est++;
ost = ost + 2;
}
}

}

void circle_display() {
        int x, y, r2, tx, ty, xy;
        r2 = mmc * mmc;
        for (x = -mmc; x <= mmc; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = 34 + x;
	ty = 17 + y;
	xy = 17 - y;
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
	printf(".");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
	printf(".");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx- xy, ty);
	if (tx-xy > 5) {
	if (tx-xy < 62) {
	if (ty > 9) {
	if (ty < 25) {
	printf(".");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx+ ty, xy);
	if (tx+ty > 5) {
	if (tx+ty < 62) {
	if (xy > 9) {
	if (xy < 25) {
	printf(".");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx+ xy, ty);
	if (tx+xy > 5) {
	if (tx+xy < 62) {
	if (ty > 9) {
	if (ty < 25) {
	printf(".");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx- ty, xy);
	if (tx-ty > 5) {
	if (tx-ty < 62) {
	if (xy > 9) {
	if (xy < 25) {
	printf(".");
	}
	}
	}
	}
}
}

void draw_circle() {
        int x, y, r2, tx, ty, xy;
        r2 = mmc * mmc;
        for (x = -mmc; x <= mmc; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = 34 + x;
	ty = 17 + y;
	xy = 17 - y;
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
	printf(".");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
	printf(".");
	}
	}
	}
	}
}
}

void draw_windows_circleb() {
	int y, r2, tx, ty, xy;
	int try;

get_eq_values();

widthm = widthm + widtha + widthb + widthc + widthd + widthe;
widthm = widthm + widthf + widthg + widthh + widthi + widthj;
widthm = widthm + widthk + widthl + widthn + widtho + widthp;
widthm = widthm + widthq + widthr + widths + widtht + widthu;

widthm = widthm / 46;

if (widthm < 1) {
widthm = 1;
}
if (widthm > 7) {
widthm = 7;
bounce_circle_eqc();

}
eqrad = widthm;

        r2 = eqrad * eqrad;
        y = (int) (sqrt(r2 - wc*wc) + 0.5);
	tx = 34 + wc;
	try = 34 - wc;
	ty = 17 + y;
	xy = 17 - y;
	xxtx = tx;
	xxtry = try;
	xxty = ty;
	xxxy = xy;

	if (wcxc > (eqrad+eqrad+eqrad+eqrad-1)) {
	wcxc = 0;
	}
	wcxc++;

	if (wcxc < (eqrad+eqrad+1)) {
	circle_sectorax();
	}
	if (wcxc < (eqrad+eqrad+1+eqrad+eqrad)) {
	if (wcxc > (eqrad+eqrad)) {
	circle_sectorbx();
	}
	}
	if (wcxc == (eqrad+eqrad)) {
	wc = -eqrad;
	}
	if (wcxc == (eqrad+eqrad+eqrad+eqrad)) {
	wc = -eqrad;
	}
}
void circle_sectorax() {
xo = 34;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

xo = 61;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

xo = 61;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

xo = 34;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

xo = 6;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

xo = 6;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

wc++;
}

void circle_sectorbx() {
xo = 34;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

xo = 61;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

xo = 61;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

xo = 34;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

xo = 6;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

xo = 6;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

wc++;
}

void draw_windows_circle() {
	int y, r2, tx, ty, xy;
	int try;
        r2 = eqrad * eqrad;
        y = (int) (sqrt(r2 - wc*wc) + 0.5);
	tx = 34 + wc;
	try = 34 - wc;
	ty = 17 + y;
	xy = 17 - y;
	xxtx = tx;
	xxtry = try;
	xxty = ty;
	xxxy = xy;

	if (wcxc > (eqrad+eqrad+eqrad+eqrad-1)) { //41
	wcxc = 0;

if (ecdir == 0) {
eqrad++;
if (eqrad > 7) {
ecdir = 1;
}
}
if (ecdir == 1) {
eqrad--;
if (eqrad < 1) {
ecdir = 0;
}
}

	}
	wcxc++;

	if (wcxc < (eqrad+eqrad+1)) { //22
	circle_sectora();
	}
	if (wcxc < (eqrad+eqrad+1+eqrad+eqrad)) { //43
	if (wcxc > (eqrad+eqrad)) {  //21
	circle_sectorb();
	}
	}
	if (wcxc == (eqrad+eqrad)) {  //21
	wc = -eqrad;
	}
	if (wcxc == (eqrad+eqrad+eqrad+eqrad)) { //42
	wc = -eqrad;
	}
}

void circle_sectora() {
if (mcnt < 701) {
xo = 34;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

xo = 61;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

xo = 34;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

xo = 6;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();
}

if (mcnt > 700) {
bounce_circle_eq();
bounce_circle_eqb();
}

wc++;
}

void circle_sectorb() {
if (mcnt < 701) {
xo = 34;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

xo = 61;
yo = 17;
xi = xxtx;
yi = xxty;
draw_line_slow();

xo = 34;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();

xo = 6;
yo = 17;
xi = xxtry;
yi = xxxy;
draw_line_slow();
}

if (mcnt > 700) {
if (mcnt < 730) {
bounce_circle_eq();
bounce_circle_eqb();
}
}

wc++;
}

void bounce_circle() {
        int x, y, r2, tx, ty, xy;
        r2 = mma * mma;
        for (x = -mma; x <= mma; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = ballx + x;
	ty = bally + y;
	xy = bally - y;
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
printf("@");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
printf("@");
	}
	}
	}
	}
}

if (bcdir == 0) {
mma++;
mmd--;
if (mma > 11) {
bcdir = 1;
}
}
if (bcdir == 1) {
mma--;
mmd++;
if (mma < 0) {
bcdir = 0;
}
}
}

void bounce_tcircle() {
int exx = tballx - mma;
int eyy = tbally - mma;

int x0 = exx+(mma);
int y0 = eyy+(mma);
int stretch = mma + mma;

int theta;
for(theta=0; theta<60; theta++) {
xo = (cos(theta)*stretch)+x0;
yo = (sin(theta)*mma)+y0;

	pspDebugScreenSetXY(xo, yo);
	if (xo > 5) {
	if (xo < 62) {
	if (yo > 9) {
	if (yo < 25) {
	printf("@");
	}
	}
	}
	}
}

xo = tballx + stretch;
yo = tbally;
	pspDebugScreenSetXY(xo, yo);
	if (xo > 5) {
	if (xo < 62) {
	if (yo > 9) {
	if (yo < 25) {
	printf(" ");
	}
	}
	}
	}
}

void bounce_circle_eq() {
        int x, y, r2, tx, ty, xy;
        r2 = eqrad * eqrad;
        for (x = -eqrad; x <= eqrad; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = xxtx + x;
	ty = xxty + y;
	xy = xxty - y;
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
printf("@");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
printf("@");
	}
	}
	}
	}
}
}

void bounce_circle_eqb() {
        int x, y, r2, tx, ty, xy;
        r2 = eqrad * eqrad;
        for (x = -eqrad; x <= eqrad; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = xxtry + x;
	ty = xxxy + y;
	xy = xxxy - y;
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
printf("@");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
printf("@");
	}
	}
	}
	}
}
}

void bounce_circle_eqc() {
        int x, y, r2, tx, ty, xy;
        r2 = eqrad * eqrad;
        for (x = -eqrad; x <= eqrad; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = 34 + x;
	ty = 17 + y;
	xy = 17 - y;
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
printf("@");
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
printf("@");
	}
	}
	}
	}
}
}

void draw_line() {
        int dx = xi - xo;
        int dy = yi - yo;

	pspDebugScreenSetXY(xo, yo);
printf("@");

        if (abs(dx) > abs(dy)) {          // slope < 1
            float m = (float) dy / (float) dx;      // compute slope
            float b = yo - m*xo;
            dx = (dx < 0) ? -1 : 1;
            while (xo != xi) {
                xo += dx;

	pspDebugScreenSetXY(xo, round(m*xo + b));
printf("@");

        }
        } else
        if (dy != 0) {                              // slope >= 1
            float m = (float) dx / (float) dy;      // compute slope
            float b = xo - m*yo;
            dy = (dy < 0) ? -1 : 1;
            while (yo != yi) {
                yo += dy;

	pspDebugScreenSetXY(round(m*yo + b), yo);
printf("@");

        }
        }
}

void draw_line_slow() { // draw line while checking for interface screen edges
        int dx = xi - xo;
        int dy = yi - yo;
	int bufsum;
	
	pspDebugScreenSetXY(xo, yo);
	if (xo > 5) {
	if (xo < 62) {
	if (yo > 9) {
	if (yo < 25) {

if (fsshow == 1) {
if (amiga == 1) {
printf("%c",dia);	
}
}
else {
	printf("+");
}
	}
	}
	}
	}

        if (abs(dx) > abs(dy)) {          // slope < 1
            float m = (float) dy / (float) dx;      // compute slope
            float b = yo - m*xo;
            dx = (dx < 0) ? -1 : 1;
            while (xo != xi) {
                xo += dx;

bufsum = round(m*xo + b);

	pspDebugScreenSetXY(xo, bufsum);
	if (xo > 5) {
	if (xo < 62) {
	if (bufsum > 9) {
	if (bufsum < 25) {
if (fsshow == 1) {
if (amiga == 1) {
printf("%c",dia);	
}
}
else {
	printf("+");
}
	}
	}
	}
	}
        }
        } else
        if (dy != 0) {                              // slope >= 1
            float m = (float) dx / (float) dy;      // compute slope
            float b = xo - m*yo;
            dy = (dy < 0) ? -1 : 1;
            while (yo != yi) {
                yo += dy;

bufsum = round(m*yo + b);

	pspDebugScreenSetXY(bufsum, yo);
	if (bufsum > 5) {
	if (bufsum < 62) {
	if (yo > 9) {
	if (yo < 25) {

if (fsshow == 1) {
if (amiga == 1) {
printf("%c",dia);	
}
}
else {
	printf("+");
}
	}
	}
	}
	}
        }
        }
}

void draw_bar() {
        int dx = xi - xo;
        int dy = yi - yo;

	pspDebugScreenSetXY(xo, yo);
	printf("/\\");

        if (abs(dx) > abs(dy)) {          // slope < 1
            float m = (float) dy / (float) dx;      // compute slope
            float b = yo - m*xo;
            dx = (dx < 0) ? -1 : 1;
            while (xo != xi) {
                xo += dx;

	pspDebugScreenSetXY(xo, round(m*xo + b));
	printf("/\\");

            }
        } else
        if (dy != 0) {                              // slope >= 1
            float m = (float) dx / (float) dy;      // compute slope
            float b = xo - m*yo;
            dy = (dy < 0) ? -1 : 1;
            while (yo != yi) {
                yo += dy;

	pspDebugScreenSetXY(round(m*yo + b), yo);
	printf("/\\");
            }
}
}

void get_eq_values() {
widtha = Voice_RealVolume(2) / 1759;
widthb = Voice_RealVolume(4) / 1759;
widthc = Voice_RealVolume(8) / 1759;
widthd = Voice_RealVolume(9) / 1759;
widthe = Voice_RealVolume(10) / 1759;
widthf = Voice_RealVolume(12) / 1759;
widthg = Voice_RealVolume(14) / 1759;
widthh = Voice_RealVolume(15) / 1759;
widthi = Voice_RealVolume(16) / 1759;
widthj = Voice_RealVolume(17) / 1759;
widthk = Voice_RealVolume(18) / 1759;
widthl = Voice_RealVolume(19) / 1759;
widthm = Voice_RealVolume(20) / 1759;
widthn = Voice_RealVolume(21) / 1759;
widtho = Voice_RealVolume(22) / 1759;
widthp = Voice_RealVolume(23) / 1759;
widthq = Voice_RealVolume(24) / 1759;
widthr = Voice_RealVolume(25) / 1759;
widths = Voice_RealVolume(26) / 1759;
widtht = Voice_RealVolume(27) / 1759;
widthu = Voice_RealVolume(28) / 1759;
widthv = Voice_RealVolume(29) / 1759;
widthw = Voice_RealVolume(30) / 1759;
widthx = Voice_RealVolume(31) / 1759;
}

void draw_demo_circle() {
	int deep;

	if (mcnt > 770) {
	rotate_ver();
	}
	if (mcnt > 790) {
	rotate_ver();
	}

	for(deep=4; deep<26; deep++) { // 3, 25
	rotate_ver();
        int x, y, r2, tx, ty, xy;
        r2 = deep * deep;
        for (x = -deep; x <= deep; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = dcent + x;
	ty = 17 + y;
	xy = 17 - y;
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
	printf("%c",xverx);
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
	printf("%c",xverx);
	}
	}
	}
	}
}
}
}

void rotate_ver() {
xverx = sver[0];
sver[0] = sver[1];
sver[1] = sver[2];
sver[2] = sver[3];
sver[3] = sver[4];
sver[4] = sver[5];
sver[5] = sver[6];
sver[6] = sver[7];
sver[7] = sver[8];
sver[8] = sver[9];
sver[9] = sver[10];
sver[10] = sver[11];
sver[11] = sver[12];
sver[12] = sver[13];
sver[13] = sver[14];
sver[14] = sver[15];
sver[15] = sver[16];
sver[16] = sver[17];
sver[17] = sver[18];
sver[18] = sver[19];
sver[19] = sver[20];
sver[20] = sver[21];
sver[21] = sver[22];
sver[22] = sver[23];
sver[23] = sver[24];
sver[24] = xverx;
}

void copy_tunnel() {
sver[0] = 0x20;
sver[1] = 0x20;
sver[2] = 0x5C;
sver[3] = 0x20;
sver[4] = 0x20;
sver[5] = 0x20;
sver[6] = 0x7C;
sver[7] = 0x20;
sver[8] = 0x20;
sver[9] = 0x20;
sver[10] = 0x20;
sver[11] = 0x5F;
sver[12] = 0x2F;
sver[13] = 0x20;
sver[14] = 0x20;
sver[15] = 0x20;
sver[16] = 0x20;
sver[17] = 0x20;
sver[18] = 0x7C;
sver[19] = 0x20;
sver[20] = 0x20;
sver[21] = 0x20;
sver[22] = 0x20;
sver[23] = 0x2D;
sver[24] = 0x2D;
}

void print_verify_passed() {
pspDebugScreenSetTextColor(colord);
pspDebugScreenSetXY(18, 28);
printf("         VERIFY PASSED ");
}

void print_verify_failed() {
print_blankfooter();
pspDebugScreenSetTextColor(colord);
pspDebugScreenSetXY(18, 28);
printf("         VERIFY FAILED! ");
fails ++; // increment value for verify fails.
}

u32 ramAvailableLineareMax () {
 u32 size, sizeblock; 
 u8 *ram; 

 // Init variables 
 size = 0; 
 sizeblock = RAM_BLOCK; 

 // Check loop 
 while (sizeblock) 
 { 
  // Increment size 
  size += sizeblock; 

  // Allocate ram 
  ram = malloc(size); 

  // Check allocate 
  if (!(ram)) 
  { 
   // Restore old size 
   size -= sizeblock; 

   // Size block / 2 
   sizeblock >>= 1; 
  } 
  else 
   free(ram); 
 } 

 return size; 
} 

u32 GetRAMFree() { 
 u8 **ram, **temp; 
 u32 size, count, x;  
 ram = NULL; 
 size = 0; 
 count = 0; 

 for (;;) 
 { 

  if (!(count % 10)) 
  { 
   temp = realloc(ram,sizeof(u8 *) * (count + 10)); 
   if (!(temp)) break; 
   ram = temp; 
   size += (sizeof(u8 *) * 10); 
  } 
  x = ramAvailableLineareMax(); 
  if (!(x)) break; 
  ram[count] = malloc(x); 
  if (!(ram[count])) break; 
  size += x; 
  count++; 
 } 

 if (ram) 
 { 
  for (x=0;x<count;x++) free(ram[x]); 
  free(ram); 
 } 

	int i;
	if (started == 0) {
	pspDebugScreenSetXY(16, 28);
	printf("  FREE SYSTEM RAM: %d BYTES", size);
	for(i=0; i<50; i++) {
	sceDisplayWaitVblankStart();
	}
	} // started

	if (started == 0) {
	if (size < 13900000) { // allow some RAM consumption for prx files
	boot_mem();
    pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	} // started

	if (started == 1) {
	if (size < 4400000) { //
	boot_mikmem();
    pspAudioEnd();
	sceKernelExitGame();
	return 0;
	}
	} // started

return size;
}

void check_nick() {
	int idc;
	int sdrg;
	int ncnt;

	sdrg = sceIoOpen("flash1:/registry/system.dreg", PSP_O_RDONLY, 0777);
	sceIoRead(sdrg,dreg,11360);
	sceIoClose(sdrg);
	art = 1;
	sturm = 1;
	prelim = 1;
	irshell = 1;
	ascii = 1;
	rawdump = 1;

if (dreg[2432] == 0) {
nickaddy = 1;
}

if (dreg[7552] == 0) {
if (dreg[2432] == 0) {
nickaddy = 2;
}
}

if (nickaddy == 2) {
	dcnt = 9088; 		// set to beginning of nickname offset in flash1 for 3.02 OE
}
if (nickaddy == 1) {
	dcnt = 7552; 		// set to beginning of nickname offset in flash1 for 2.71 SE
}
if (nickaddy == 0) {
	dcnt = 2432; 		// set to beginning of nickname offset in flash1 for 1.50
}

	if (nickaddy == 0) {
	if (dreg[2434] != 0x61) { 	// a
	sturm = 0;
	}
	if (dreg[2434] != 0x63) { 	// c
	ascii = 0;
	}
	if (dreg[2435] != 0x69) { 	// i
	ascii = 0;
	}
	if (dreg[2436] != 0x69) { 	// i
	prelim = 0;
	}
	if (dreg[2436] != 0x69) { 	// i
	ascii = 0;
	}
	if (dreg[2432] != 0x41) {	// A
	art = 0;
	}
	if (dreg[2432] != 0x72) {	// r
	rawdump = 0;
	}
	if (dreg[2432] != 0x61) {	// a
	ascii = 0;
	}
	if (dreg[2433] != 0x61) {	// a
	rawdump = 0;
	}
	if (dreg[2432] != 0x69) {	// i
	irshell = 0;
	}
	if (dreg[2435] != 0x6C) { 	// l
	prelim = 0;
	}
	if (dreg[2438] != 0x6C) { 	// l
	irshell = 0;
	}
	if (dreg[2433] != 0x68) { 	// h
	sturm = 0;
	}
	if (dreg[2434] != 0x77) { 	// w
	rawdump = 0;
	}
	if (dreg[2433] != 0x73) { 	// s
	ascii = 0;
	}
	if (dreg[2432] != 0x50) {	// P
	prelim = 0;
	}
	if (dreg[2434] != 0x53) { 	// S
	irshell = 0;
	}
	if (dreg[2434] != 0x65) { 	// e
	prelim = 0;
	}
	if (dreg[2439] != 0x00) { 	// blank
	irshell = 0;
	}
	if (dreg[2435] != 0x00) { 	// blank
	art = 0;
	}
	if (dreg[2435] != 0x68) { 	// h
	irshell = 0;
	}
	if (dreg[2436] != 0x65) { 	// e
	irshell = 0;
	}
	if (dreg[2435] != 0x75) { 	// u
	sturm = 0;
	}
	if (dreg[2433] != 0x52) { 	// R
	irshell = 0;
	}
	if (dreg[2438] != 0x00) { 	// blank
	prelim = 0;
	}
	if (dreg[2432] != 0x53) {	// S
	sturm = 0;
	}
	if (dreg[2437] != 0x6D) { 	// m
	prelim = 0;
	}
	if (dreg[2437] != 0x6C) { 	// l
	irshell = 0;
	}
	if (dreg[2436] != 0x6E) { 	// n
	sturm = 0;
	}
	if (dreg[2433] != 0x72) { 	// r
	art = 0;
	prelim = 0;
	}
	if (dreg[2434] != 0x74) { 	// t
	art = 0;
	}
	if (dreg[2437] != 0x00) { 	// blank
	sturm = 0;
	}
	} // nickaddy

	if (nickaddy == 1) {
	if (dreg[7552] != 0x72) {	// r
	rawdump = 0;
	}
	if (dreg[7554] != 0x77) { 	// w
	rawdump = 0;
	}
	if (dreg[7553] != 0x61) {	// a
	rawdump = 0;
	}
	} // nickaddy

	if (nickaddy == 2) {
	if (dreg[9088] != 0x72) {	// r
	rawdump = 0;
	}
	if (dreg[9090] != 0x77) { 	// w
	rawdump = 0;
	}
	if (dreg[9089] != 0x61) {	// a
	rawdump = 0;
	}
	} // nickaddy

	if (irshell == 1) {		// activate iRShell program allowed status
	sturm = 1;			//
	}				//

	for(ncnt=0; ncnt<25; ncnt++) {	// erase nickname buffer
	nickname[ncnt] = 0;		//
	}				//

	for(ncnt=0; ncnt<25; ncnt++) {	// copy nickname to nickname buffer
	nickname[ncnt] = dreg[dcnt];	//
	dcnt++;				// increment reg file address pointer
	}				//

	dcnt = 0; 			// we will recycle this counter

	for(ncnt=0; ncnt<25; ncnt++) {  // find the length of the nickname
	if (nickname[ncnt] == 0) {	// look for zero value byte
	break;				// loop will be broken when the first zero is found
	}				//
	dcnt++;				// dcnt counts the size of the nickname
	}				//

	ncnt = 0;			// reset counter to be used for nickname array pointer

	pspDebugScreenSetBackColor(0x00000000); // set up screen colours
	pspDebugScreenSetTextColor(0x00EFDDCA); //
	pspDebugScreenSetXY(31 - (dcnt/2), 17); // set up screen centering

	printf("Hello ");		// print the word "Hello", with a space.

	for(idc=0; idc<dcnt; idc++) {	// do loop for number of characters in the nickname
	printf("%c",nickname[ncnt]);	// print a character of the nickname
	ncnt++;				// increment character print counter
	}				//
}					// done with nickname

void check_front_buttons() {
	SceCtrlData pad;
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	sceCtrlReadBufferPositive(&pad, 1);
	if (pad.Buttons & PSP_CTRL_HOME) {
	pspDebugScreenSetXY(0, 17);
	printf("                       HOME BUTTON IS STUCK DOWN      ");
	button_disabled();
	dishome = 1;
	}
	if (pad.Buttons & PSP_CTRL_START) {
	pspDebugScreenSetXY(0, 17);
	printf("                       START BUTTON IS STUCK DOWN     ");
	button_disabled();
	disstart = 1;
	}
	if (pad.Buttons & PSP_CTRL_SELECT) {
	pspDebugScreenSetXY(0, 17);
	printf("                      SELECT BUTTON IS STUCK DOWN     ");
	button_disabled();
	disselect = 1;
	}
}

void button_disabled() {
	int i = 0;
	pspDebugScreenSetXY(0, 19);
	printf("                    BUTTON DISABLED FOR THIS SESSION  ");
	for(i=0; i<140; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetXY(0, 17);
	printf("                                                      ");
	pspDebugScreenSetXY(0, 19);
	printf("                                                      ");
	for(i=0; i<30; i++) {
	sceDisplayWaitVblankStart();
	}
}

void labels_check() {
	int chml;
	int labl;
	int lab;

	labl = sceIoOpen("./Xflash/Modified/Labels.XF", PSP_O_RDONLY, 0777);
	if(labl >= 0) {
	labfileexists = 1;
	}

	if (labfileexists == 1) {
	chml = sceIoOpen("./Xflash/Modified/Labels.XF", PSP_O_RDONLY, 0777);
	labxfsize = sceIoLseek32(chml, 0, SEEK_END);
	sceIoClose(chml);

	lab = sceIoOpen("./Xflash/Modified/Labels.XF", PSP_O_RDONLY, 0777);
	sceIoRead(lab,ebufferxor,261);
	sceIoClose(lab);

	checksum = 0x3F;						// write start value to checksum buffer
	for(counter=0; counter<259; counter++) {			// calculate checksum
	checksum = checksum ^ ebufferxor[counter];			//
	}								//
	
	if (ebufferxor[260] != checksum) {				// compare RAM checksum to file checksum
	labelsxffail = 1;						// set check fail status
	}
	}
	set_ebufferxor();						// reset random xor value array
}

void theme_check() {
	int chmt;

	chmt = sceIoOpen("./Xflash/Xflash/Theme.XF", PSP_O_RDONLY, 0777);
	themexfsize = sceIoLseek32(chmt, 0, SEEK_END);
	sceIoClose(chmt);
}

void read_labels() {
	int lab;
	int d;
	int labv;
	int fileexists = 0;

	labv = sceIoOpen("./Xflash/Modified/Labels.XF", PSP_O_RDONLY, 0777);
	if(labv >= 0) {
	fileexists = 1;
	}

if (fileexists == 0) {
				print_blankfooter();
				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(17, 28);
				printf("     LABEL FILE DOES NOT EXIST");
			        for(d=0; d<120; d++) {
		                sceDisplayWaitVblankStart();
				}
				print_blankfooter();
				editor_footer();
}
if (fileexists == 1) {
				print_blankfooter();
				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(17, 28);
				printf("        READING LABEL FILE ");
			        for(d=0; d<120; d++) {
		                sceDisplayWaitVblankStart();
				}
				print_blankfooter();
				editor_footer();

	lab = sceIoOpen("./Xflash/Modified/Labels.XF", PSP_O_RDONLY, 0777);
	sceIoRead(lab,ebuffer,261);
	sceIoClose(lab);

	checksum = 0x3F;						// write start value to checksum buffer
	for(counter=0; counter<259; counter++) {			// calculate checksum
	checksum = checksum ^ ebuffer[counter];				//
	}								//

	for(counter=0; counter<260; counter++) {			// xor with default value array
	ebuffer[counter] = ebuffercomp[counter] ^ ebuffer[counter];	//
	}								//

	for(counter=0; counter<260; counter++) {			// xor with random value array
	ebuffer[counter] = ebufferxor[counter] ^ ebuffer[counter];	//
	}								//
}
}

void play_freespace() {		// play Free Space sub game X> <><><><><><> :)
SceCtrlData cont;

fs_enemy_explosion();		// check status of explosions
fs_player_explosion();		//

if (fsfire == 0) {		// reset backward laser length for special weapon
lasertime = 3;			// define time laser lasts per shot
stretch = -2;
}

if (fsalive == 1) {
if (fsshow == 1) {
	pspDebugScreenSetTextColor(tripcol);	// draw free space enemy
	pspDebugScreenSetXY(ballx, bally);
	printf("%c",bsmiley);
}
}

if (fsover == 0) {		// enemy collision detection
if (fsshow == 1) {
if (fsalive == 1) {
if (fsx == ballx) {
if (fsy == bally) {
	fsover = 1;
	fsphit = 1;		// tell main routine to make sound
fs_player_destroyed();
}
}

if (fsx + 1 == ballx) {
if (fsy == bally) {
	fsover = 1;
	fsphit = 1;		// tell main routine to make sound
fs_player_destroyed();
}
}
}
}
}

if (fsover == 0) {
if (fsfire == 0) {
	fbx = fsx;
	fby = fsy;
}

if (fsfire == 1) {

if (amiga == 1) {
lasertime--;
if (lasertime < 1) {
fsfire = 0;
}
}

	fbx++;
	if (fbx > 60) {
	fsfire = 0;
	}
	else {
	fs_bullet_collision();
	}

	fbx++;
	if (fbx > 60) {
	fsfire = 0;
	}
	else {

if (amiga == 0) {
	pspDebugScreenSetTextColor(0x005569FE);	// draw red bullet
}
else {
	pspDebugScreenSetTextColor(0x00FE6955);	// or blue laser
}

	pspDebugScreenSetXY(fbx - 1, fby);

if (amiga == 1) {
stretch++;
stretch++;

if (fs_left == 1) {
xo = fbx - (stretch + 3);
}
else {
xo = fbx - stretch;
}

yo = fby;
xi = 59;
yi = fby;
}

if (fsfire == 1) {				// don't count two collisions for one shot
if (amiga == 1) {
draw_line_slow();
}
	printf("%c%c",dia,dia);			// and kill bullet for collision
	fs_bullet_collision();			//
}
}
}
}

if (fsover == 0) {
if (fsshow == 1) {
if (fsfire == 1) {
	pspDebugScreenSetTextColor(0x005569FE);	// draw red free space ship
}
if (fsfire == 0) {
	pspDebugScreenSetTextColor(0x0055FE69);	// draw green free space ship
}
	pspDebugScreenSetXY(fsx, fsy);
	printf("X>");

	if (mcnt == 407) {			// activate coin if player hasn't won yet
	coinactive = 1;				// while inside a loop that is only active during gameplay
	}					//
}
}

	if (coinactive == 1) {			// move coin across screen
	coinx--;				//
	coinx--;				//

	if (coinx < 28) {			// gravity for coin lowers it
	coiny++;				//
	}					//

	pspDebugScreenSetTextColor(0x00EE5945);	// blue coin colour
	pspDebugScreenSetXY(coinx, coiny);	//
	printf("@");				//
	if (coinx < 7) {			// reset coin active status if it was missed
	coinactive = 0;				//
	coinx = 63;				// reset coin coordinate x
	coiny = 12;				// reset coin coordinate y
	}					//
	}


	if (fsshow == 1) {			// coin collision detection
	if (fsover == 0) {

	if (fsy == coiny) {
	if (fsx == coinx) {
	coin_inner();				//
	}

	if (fsx == coinx + 1) {
	coin_inner();				//
	}

	if (fsx == coinx - 1) {
	coin_inner();				//
	}
	}

	if (fsx == coinx) {
	if (fsy == coiny - 1) {
	coin_inner();				//
	}

	if (fsy == coiny + 1) {
	coin_inner();				//
	}
	}

	if (fsy == coiny) {
	if (fsx + 1 == coinx - 1) {
	coin_inner();				//
	}
	}

	if (fsx + 1 == coinx) {
	if (fsy == coiny - 1) {
	coin_inner();				//
	}

	if (fsy == coiny + 1) {
	coin_inner();				//
	}
	}

	}
	}

		pspDebugScreenSetTextColor(0x00EFDDCA);

		sceCtrlReadBufferPositive(&cont, 1);  // look for directional button pushes
fs_up = 0;
fs_down = 0;
fs_left = 0;
fs_right = 0;
if (fsshow == 1) {
		if (cont.Buttons & PSP_CTRL_DOWN) {
fs_down = 1;
		fsy++;
if (fsy > 24) {
fsy = 24;

if (fsover == 0) {		// floor collision detection
if (fsalive == 1) {
fsover = 1;
fsphit = 1;
fs_player_destroyed();
}
}
}
		}
		if (cont.Buttons & PSP_CTRL_UP) {
fs_up = 1;
		fsy--;
if (fsy < 10) {
fsy = 10;

if (fsover == 0) {		// roof collision detection
if (fsalive == 1) {
fsover = 1;
fsphit = 1;
fs_player_destroyed();
}
}
}
		}
}

		if (cont.Buttons & PSP_CTRL_RIGHT) {
if (fsshow == 1) {
fs_right = 1;
}
		fsx++;
if (fsx > 60) {
fsx = 60;
}
		}
		if (cont.Buttons & PSP_CTRL_LEFT) {
if (fsshow == 1) {
fs_left = 1;
}
		fsx--;
if (fsx < 6) {
fsx = 6;
}
		}

if(cont.Lx < 20) {
if (fsshow == 1) {
fs_left = 1;
}
		fsx--;
if (fsx < 6) {
fsx = 6;
}
		}

if(cont.Lx > 235) {
if (fsshow == 1) {
fs_right = 1;
}
		fsx++;
if (fsx > 60) {
fsx = 60;
}
		}
if (fsshow == 1) {
if(cont.Ly < 20) {
fs_up = 1;
		fsy--;
if (fsy < 10) {
fsy = 10;

if (fsover == 0) {		// roof collision detection
if (fsalive == 1) {
fsover = 1;
fsphit = 1;
fs_player_destroyed();
}
}
}
		}

if(cont.Ly > 235) {
fs_down = 1;
		fsy++;
if (fsy > 24) {
fsy = 24;

if (fsover == 0) {		// floor collision detection
if (fsalive == 1) {
fsover = 1;
fsphit = 1;
fs_player_destroyed();
}
}
}
		}
}

if (mcnt > 199) { // prevent fire button from working outside of freespace demo

		if (cont.Buttons & PSP_CTRL_CROSS) {
		fsshow = 1;
		fsfire = 1;
		}
}
		}

void five_lines() {
	int xmp;
	for(xmp=0; xmp<5; xmp++) {
	printf("\n                                                           ");
}
}

void fs_hitreact() {
if (fsfire == 1) {	// don't count two hits for one shot
fsehit = 1;		// tell main routine to make sound for enemy hit
fshits++;		// advance number of hits enemy has taken
fsfire = 0;		// stop bullet for collision

if (fshits == 1) {
fs_keepitup();
}

if (fshits == 3) {
fs_twohitsleft();
}

if (fshits == 4) {
fs_onehitleft();
}

if (fshits == 5) {	// number of hits required
fsehitx = 1;		// tell main routine to make sound for enemy destroyed
fshits = 0;		//
fsalive = 0;		// enemy was destroyed
fs_destroyed();		//
save_freespace();	//
}
}
}

void fs_bullet_collision() {
if (amiga == 0) {
if (fsalive == 1) {
	if (ballx == fbx) { 	// bullet collision detection
	if (bally == fby) {
	fs_hitreact();
	}
	if (bally + 1 == fby) {
	fs_hitreact();
	}
	if (bally - 1 == fby) {
	fs_hitreact();
	}
	}
	if (ballx == fbx - 1) {
	if (bally + 1 == fby) {
	fs_hitreact();
	}
	if (bally - 1 == fby) {
	fs_hitreact();
	}
	if (bally == fby) {
	fs_hitreact();
	}
	}
}
}
else {
if (fsalive == 1) {
	if (bally == fby) { 		// bullet collision detection (same line as laser)
	if (ballx > fbx - stretch) {	// bullet collision detection (must not be shot behind ship)
	fs_hitreact();
	}
	}
}
}
}

void save_freespace() {
	if (freespace == 0) {	// save Free Space Easter Egg status if triggered for first time
	freespace = 1;
	fshold = 200;	// set message hold timer
	settings[0x45] = 0x31;
	save_settings();
	}
}

void fs_enemy_explosion() {
if (eexplode == 1) {
	pspDebugScreenSetTextColor(0x00586EFC);	// red for explosion
	pspDebugScreenSetXY(ballx, bally);
	printf("%c",sver[cexplode] ^ ver[cexplode]);
	cexplode--;

if (cexplode < 4) {
	pspDebugScreenSetTextColor(0x00365EEC);	// red for explosion
        int x, y, r2, tx, ty, xy;
	int iexplode;
	iexplode = 4 - cexplode;
        r2 = iexplode * iexplode;
        for (x = -iexplode; x <= iexplode; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = ballx + x;
	ty = bally + y;
	xy = bally - y;
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
	printf("%c",alien);
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
	printf("%c",alien);
	}
	}
	}
	}
}
}
}

if (cexplode == 0) {
eexplode = 0;
}
}

void fs_player_explosion() {
if (pexplode == 1) {
	pspDebugScreenSetTextColor(0x00586EFC);	// red for explosion
	pspDebugScreenSetXY(fsx, fsy);
	printf("%c",sver[cexplode] ^ ver[cexplode]);
	cexplode--;

if (cexplode < 4) {
	pspDebugScreenSetTextColor(0x00365EEC);	// red for explosion
        int x, y, r2, tx, ty, xy;
	int iexplode;
	iexplode = 4 - cexplode;
        r2 = iexplode * iexplode;
        for (x = -iexplode; x <= iexplode; x++) {
        y = (int) (sqrt(r2 - x*x) + 0.5);
	tx = fsx + x;
	ty = fsy + y;
	xy = fsy - y;
	pspDebugScreenSetXY(tx, ty);
	if (tx > 5) {
	if (tx < 62) {
	if (ty > 9) {
	if (ty < 25) {
	printf("%c",alien);
	}
	}
	}
	}
	pspDebugScreenSetXY(tx, xy);
	if (tx > 5) {
	if (tx < 62) {
	if (xy > 9) {
	if (xy < 25) {
	printf("%c",alien);
	}
	}
	}
	}
}
}
}

if (cexplode == 0) {
pexplode = 0;
}
}

void fs_message () {
if (fshold > 0) {
fshold--;
fsx = 29;				// set initial free space ship position
fsy = 15;				//
	pspDebugScreenSetTextColor(tripcol);
	pspDebugScreenSetXY(0, 17);
	printf("                    FREE SPACE IS NOW AVAILABLE IN          ");
	pspDebugScreenSetXY(0, 19);
	printf("                         THE XMB LABEL EDITOR               ");
}
}

void coin_inner() {
	amiga = 1;				// set special weapon status
	fscoin = 1;				// tell main routine to make sound
	print_amiga_weapon();			// print message
	coinactive = 0;				//
	coinx = 63;				// reset coin coordinate x
	coiny = 12;				// reset coin coordinate y
}

void fs_player_destroyed() {
	fsover = 1;				// set game over status for this round
	fsphit = 1;				// tell main routine to make sound
	pexplode = 1;				// set player explosion status
	cexplode = 12;				// set explosion counter

	pspDebugScreenSetTextColor(0x00EFDDCA);
				pspDebugScreenSetXY(13, 28);
				printf("      YOU WERE DESTROYED - GAME OVER      ");
}

void fs_destroyed() {

eexplode = 1; // set enemy explosion status
cexplode = 12; // set explosion status

fsscore = mcnt - 200;			// calculate score based on time taken
fsscore = fsscore * 40;			//
fsscore = 10800 - fsscore;		//

if (fsscore > 10200) {			// limit high score to 10200
fsscore = 10200;			// since that will be divided down to 255 (FFh)
}					// which is the maximum our buffer will allow

if (fsscore > fshiscore) { 		// compare score to high score
fshiscore = fsscore;			// update high score if current score is higher
settings[0x46] = fsscore/40;		// save high score to settings file
settings[0x46] = settings[0x46] ^ 0x5f;	// simple xor encrypt high score byte
save_settings();			//
}					//

	pspDebugScreenSetTextColor(0x00EFDDCA);
				pspDebugScreenSetXY(15, 27);
				printf("          ENEMY WAS DESTROYED           ");
				pspDebugScreenSetXY(15, 28);
				printf("           YOUR SCORE: %d             ",fsscore);
				pspDebugScreenSetXY(15, 29);
				printf("             HI SCORE: %d             ",fshiscore);
if (fsscore > 8040) {
				pspDebugScreenSetTextColor(0x005CFE6C);	// green
				pspDebugScreenSetXY(15, 30);
				printf("               T%cP %cU%c!                 ",ooo,ggg,nnn);
}

}

void fs_onehitleft() {
	pspDebugScreenSetTextColor(0x00EFDDCA);
				pspDebugScreenSetXY(15, 28);
				printf("           ONE HIT TO GO !!!            ");
}

void fs_twohitsleft() {
	pspDebugScreenSetTextColor(0x00EFDDCA);
				pspDebugScreenSetXY(15, 28);
				printf("          TWO HITS TO GO !!!            ");
}

void fs_keepitup() {
	pspDebugScreenSetTextColor(0x00EFDDCA);
				pspDebugScreenSetXY(15, 28);
				printf("       GOOD GOING ! KEEP IT UP !        ");
}

void print_amiga_weapon() {
	pspDebugScreenSetTextColor(0x00EFDDCA);
				pspDebugScreenSetXY(15, 27);
				printf("         AMIGA SUPER WEAPON !           ");
}

void reset_freespace() {
		clear_midscreen();	//
		fsover = 0;		// free space game can be restarted
		fsshow = 1;		// do not hide the game
		fsalive = 1;		// revive enemy
		fshits = 0;		// reset number of hits taken
		fsfire = 0;		// kill current bullet
		amiga = 0;		// turn off super weapon
		fsscore = 10800;	// reset score
		fsx = 8;		// reset ship position
		fsy = 17;		//
		fbx = 8;		// reset bullet position
		fby = 17;		//
		mcnt = 200;		// reset counter to starfield position
}

void demo_position() {
mcntm = mcnt;
mcrlx = crlx;			// credits bar position memory
mcrla = crla;			//
mcrlb = crlb;			//
mcrlc = crlc;			//
}

void fade_space() {
int up = 17;
int down = 17;
	for(xexp=0; xexp<7; xexp++) {
	pspDebugScreenSetXY(0, down);
	printf("                                                                ");
	pspDebugScreenSetXY(0, up);
	printf("                                                                ");
	sceDisplayWaitVblankStart();
	up++;
	down--;
	}
}

void vshmainchecknodax() {
	int chmn;

	print_blankfooter();
	for(chmn=0; chmn<10; chmn++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(18, 28);
	printf("       check vshmain.prx      ");
	for(chmn=0; chmn<70; chmn++) {
	sceDisplayWaitVblankStart();
	}

	chmn = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	vshmainsize = sceIoLseek32(chmn, 0, SEEK_END);
	sceIoClose(chmn);

if (vshmainsize == 72032) {
nodax = 1; // set no custom firmware status
}
}

void checkdaseb() {
	nottt = 0;

	if (data[cpa] != 50) {
	nottt = 1;
	}
	if (data[cpc] != 55) {
	nottt = 1;
	}
	if (data[cpd] != 49) {
	nottt = 1;
	}
}

void checkdaoea() {
	notto = 0;

	if (data[cpa] != 51) {
	notto = 1;
	}
	if (data[cpc] != 48) {
	notto = 1;
	}
	if (data[cpd] != 50) {
	notto = 1;
	}
}

void check_se() {
	int fdd;

	fdd = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	vshmainsize = sceIoLseek32(fdd, 0, SEEK_END);
	sceIoClose(fdd);

	if (vshmainsize == 26166) {
	seb = 1; // set 2.71 SE-B firmware detected status
	}

	if (vshmainsize == 26686) {
	seb = 1; // set 2.71 SE-B3 firmware detected status
	}

	if (vshmainsize == 28070) {
	seb = 1; // set 2.71 SE-C firmware detected status
	}

	if (test == 0) {
	fdd = sceIoOpen("flash0:/vsh/nodule/dd_helper.prx", PSP_O_RDONLY, 0); // check for dd-helper.prx existence
	}
	else {
	fdd = sceIoOpen("./Xflash/Dump/flash0/vsh/nodule/dd_helper.prx", PSP_O_RDONLY, 0777);
	}

	if(fdd < 0) {
	seb = 0;
        }
	sceIoClose(fdd);
}

void check_oe() {
	int fdd;

	fdd = sceIoOpen("flash0:/vsh/module/vshmain.prx", PSP_O_RDONLY, 0777);
	vshmainsize = sceIoLseek32(fdd, 0, SEEK_END);
	sceIoClose(fdd);

	if (vshmainsize == 30138) {
	seb = 2; // set 3.02 OE-A firmware detected status
	}

	if (test == 0) {
	fdd = sceIoOpen("flash0:/vsh/nodule/dd_helper.prx", PSP_O_RDONLY, 0); // check for dd-helper.prx existence
	}
	else {
	fdd = sceIoOpen("./Xflash/Dump/flash0/vsh/nodule/dd_helper.prx", PSP_O_RDONLY, 0777);
	}

	if(fdd < 0) {
	seb = 0;
        }
	sceIoClose(fdd);
}

void print_ua_footer() {

	int ixts;
	finger_patch();
	print_blankfooter();

	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(12, 27);
	printf("This feature is only available for use with");

if (stingoe == 0) {
	pspDebugScreenSetXY(23, 28);
	printf("1.50 firmware PSP units.");
}
else {
	pspDebugScreenSetXY(12, 28);
	printf("original 1.50 & 2.71 SE firmware PSP units.");
}

	finger_stop_anim();

		for(ixts=0; ixts<90; ixts++) {
		sceDisplayWaitVblankStart();
		}

	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(26, 30);
	printf("Downgrade Today!!!");
		for(ixts=0; ixts<150; ixts++) {
		sceDisplayWaitVblankStart();
		}
	sting = 0; // reset sting message flags
	stingoe = 0; // 
}

void en1337_sysctrl() {
	int fdx;
		if (test == 0) {
		fdx = sceIoOpen("flash0:/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
		else {
		fdx = sceIoOpen("./Xflash/Dump/flash0/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fdx,sysctl,65454);
	sceIoClose(fdx);

	chkk = 1;

	sysctl[0xA47C] = 0x31;	// set version spoof
	sysctl[0xA47E] = 0x33;
	sysctl[0xA480] = 0x33;
	sysctl[0xA482] = 0x37;
	sysctl[0xA486] = 0x20;	// blank out letters
	sysctl[0xA488] = 0x20;
	sysctl[0xA48A] = 0x20;
	sysctl[0xA48C] = 0x20;
}

void orig_sysctrl() {
	int fdx;
		if (test == 0) {
		fdx = sceIoOpen("flash0:/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
		else {
		fdx = sceIoOpen("./Xflash/Dump/flash0/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fdx,sysctl,65454);
	sceIoClose(fdx);

	chkk = 1;

	sysctl[0xA47C] = 0x33;	// set original 3.02 OE-B version
	sysctl[0xA47E] = 0x2E;
	sysctl[0xA480] = 0x30;
	sysctl[0xA482] = 0x32;
	sysctl[0xA484] = 0x20;
	sysctl[0xA486] = 0x4F;
	sysctl[0xA488] = 0x45;
	sysctl[0xA48A] = 0x2D;
	sysctl[0xA48C] = 0x42;
}

void oea_sysctrl() {
	int fdx;
		if (test == 0) {
		fdx = sceIoOpen("flash0:/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
		else {
		fdx = sceIoOpen("./Xflash/Dump/flash0/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fdx,sysctl,65454);
	sceIoClose(fdx);

	chkk = 1;

	sysctl[0xA47C] = 0x33;	// set original 3.02 OE-A version
	sysctl[0xA47E] = 0x2E;
	sysctl[0xA480] = 0x30;
	sysctl[0xA482] = 0x32;
	sysctl[0xA484] = 0x20;
	sysctl[0xA486] = 0x4F;
	sysctl[0xA488] = 0x45;
	sysctl[0xA48A] = 0x2D;
	sysctl[0xA48C] = 0x41;
}

void oec_sysctrl() {
	int fdx;
		if (test == 0) {
		fdx = sceIoOpen("flash0:/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
		else {
		fdx = sceIoOpen("./Xflash/Dump/flash0/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fdx,sysctl,65454);
	sceIoClose(fdx);

	chkk = 1;

	sysctl[0xA47C] = 0x33;	// set original 3.02 OE-C version
	sysctl[0xA47E] = 0x2E;
	sysctl[0xA480] = 0x30;
	sysctl[0xA482] = 0x32;
	sysctl[0xA484] = 0x20;
	sysctl[0xA486] = 0x4F;
	sysctl[0xA488] = 0x45;
	sysctl[0xA48A] = 0x2D;
	sysctl[0xA48C] = 0x43;
}

void read_sysctrl() {
	int fdx;
		if (test == 0) {
		fdx = sceIoOpen("flash0:/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
		else {
		fdx = sceIoOpen("./Xflash/Dump/flash0/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fdx,sysctl,65454);
	sceIoClose(fdx);

	chkk = 1;

	sysctl[0xA47C] = data[cpa];	// set version spoof
	sysctl[0xA480] = data[cpc];
	sysctl[0xA482] = data[cpd];
	sysctl[0xA486] = 0x20;		// blank out letters
	sysctl[0xA488] = 0x20;
	sysctl[0xA48A] = 0x20;
	sysctl[0xA48C] = 0x20;
}

void write_sysctrl() {
	int voa;
	int goa;
		if (test == 0) {
		goa = sceIoOpen("flash0:/kn/systemctrl.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		goa = sceIoOpen("./Xflash/Dump/flash0/kn/systemctrl.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(goa,sysctl,65454);
	sceIoClose(goa);

	for(voa=0; voa<60; voa++) {
	sceDisplayWaitVblankStart();
	}
}

void verify_sysctl() {
	int opy;
	int fopx;
	int opl;

	if (test == 0) {
	fopx = sceIoOpen("flash0:/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
	}
	else {
	fopx = sceIoOpen("./Xflash/Dump/flash0/kn/systemctrl.prx", PSP_O_RDONLY, 0777);
	}

	sceIoRead(fopx,vsysctl,65454);
	sceIoClose(fopx);

	verify = 0;
	for(opl=0; opl<65454; opl++) {
		if (sysctl[opl] != vsysctl[opl]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/kn/systemctrl.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/kn/systemctrl.prx", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,sysctl,65454);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		verify_sysctl();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(opy=0; opy<60; opy++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void print_information() {

	sceIdStorageReadLeaf(0x100, Country);
	sceIdStorageReadLeaf(0x045, Region);
	sceIdStorageReadLeaf(0x050, Serial);
	sceIdStorageReadLeaf(0x044, Mac);
	sceIdStorageReadLeaf(0x041, USBdes);

	mac0 = Mac[0];
	mac1 = Mac[1];
	mac2 = Mac[2];
	mac3 = Mac[3];
	mac4 = Mac[4];
	mac5 = Mac[5];

	clear_midscreen();	//


	print_header();		//
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';   Most of this information is read from the  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;    id storage area on your PSP's NAND chip.  ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;                                              ;.-.;");
	pspDebugScreenSetXY(0, 30); 
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetTextColor(colord);


coco = 0;

for(xgt=0; xgt<20; xgt++) {
sceDisplayWaitVblankStart();
}

print_coco();
printf(" (1000 = Japan)              ");

if (Country[61] == 0x03) {
coco = 1;
}


if (coco == 0) {
print_coco();
printf(" (1001 = USA)                ");
}

if (Country[61] == 0x04) {
coco = 1;
}

if (coco == 0) {
print_coco();
printf(" (1002 = Australia)          ");
}

if (Country[61] == 0x09) {
coco = 1;
}

if (coco == 0) {
print_coco();
printf(" (1004 = Europe)             ");
}

if (Country[61] == 0x05) {
coco = 1;
}

if (coco == 0) {
print_coco();
printf(" (1005 = Korea)              ");
}

if (Country[61] == 0x06) {
coco = 1;
}

if (coco == 0) {
print_coco();
printf(" (1006 = Hong Kong/Singapore)");
}

if (Country[61] == 0x0A) {
coco = 1;
}

if (coco == 0) {
print_coco();
printf(" (???? = Undefined)          ");
}

while (1) { //

	pspDebugScreenSetXY(16, 12);
	printf(" Country Code: %02X",Country[61]);

	if(Country[61] == 0x03){
		printf(" (1000 = Japan)              ");
        }else if(Country[61] == 0x04){
		printf(" (1001 = USA)                ");
        }else if(Country[61] == 0x09){
		printf(" (1002 = Australia)          ");
        }else if(Country[61] == 0x05){
		printf(" (1004 = Europe)             ");
        }else if(Country[61] == 0x06){
		printf(" (1005 = Korea)              ");
        }else if(Country[61] == 0x0A){
		printf(" (1006 = Hong Kong/Singapore)");
       	}else {	
		printf(" (???? = Undefined)          ");
	}

	pspDebugScreenSetXY(16, 14);
	printf("  MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",mac0,mac1,mac2,mac3,mac4,mac5);
	pspDebugScreenSetXY(16, 16);
	printf(" Manufacturer: %c%c%c%c",USBdes[6],USBdes[8],USBdes[10],USBdes[12]);

	pspDebugScreenSetXY(16, 20);
	printf(" topmenu_plugin.rco: %d bytes",topmenu_size);


 		SceCtrlData info;				// 'info' var for information screen exit pad
		sceCtrlReadBufferPositive(&info, 1);		// detect button hold to exit information screen

		if (info.Buttons & PSP_CTRL_UP) {

	pspDebugScreenSetXY(6, 18);
	printf("         Serial: %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",Serial[0],Serial[1],Serial[2],Serial[3],
	Serial[4],Serial[5],Serial[6],Serial[7],Serial[8],Serial[9],Serial[10],Serial[11],Serial[12],Serial[13],Serial[14],Serial[15],
	Serial[16],Serial[17],Serial[18],Serial[19],Serial[20],Serial[21],Serial[22],Serial[23],Serial[24],Serial[25],Serial[26],Serial[27],
	Serial[28],Serial[29]);

	print_header();		//
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';   Most of this information is read from the  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;    id storage area on your PSP's NAND chip.  ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                  ?    ?    ?                 ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;      These data fields are experimental.     ;.-.;");
	pspDebugScreenSetXY(0, 30); 
	printf("      ;`-';                  ?    ?    ?                 ;`-';");
	pspDebugScreenSetTextColor(colord);

		}
		else {

	pspDebugScreenSetXY(6, 18);
	printf("                                               ");

	print_header();		//
	pspDebugScreenSetXY(0, 26);
	printf("      ;`-';   Most of this information is read from the  ;`-';");
	pspDebugScreenSetXY(0, 27);
	printf("      ;.-.;    id storage area on your PSP's NAND chip.  ;.-.;");
	pspDebugScreenSetXY(0, 28);
	printf("      ;`-';                                              ;`-';");
	pspDebugScreenSetXY(0, 29);
	printf("      ;.-.;               X-Flash - Art 2006!            ;.-.;");
	pspDebugScreenSetXY(0, 30); 
	printf("      ;`-';       Press Circle to exit this screen.      ;`-';");
	pspDebugScreenSetTextColor(colord);

		}

		if (info.Buttons & PSP_CTRL_CIRCLE) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		break;
		}

		if (info.Buttons & PSP_CTRL_START) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		break;
		}

		if (info.Buttons & PSP_CTRL_HOME) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
		break;
		}

		if (info.Buttons & PSP_CTRL_CROSS) {

 		SceCtrlData ainfo;				// 'info' var for information screen exit pad
		sceCtrlReadBufferPositive(&ainfo, 1);		// detect button hold to exit information screen

		if (ainfo.Buttons & PSP_CTRL_LTRIGGER) {

 		SceCtrlData binfo;				// 'info' var for information screen exit pad
		sceCtrlReadBufferPositive(&binfo, 1);		// detect button hold to exit information screen

		if (binfo.Buttons & PSP_CTRL_RTRIGGER) {
				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 6; // select button press sample
				ms_check_topmenu();

int stop_topmenu = 0;
//if (ms_topmenu_size != 247988) {	// check actual size of topmenu_plugin.rco
//if (ms_topmenu_size != 193104) {	// 
//if (ms_topmenu_size != 230856) {	// 
//if (ms_topmenu_size != 210836) {	// 
//stop_topmenu = 1;
//}
//}
//}
//}

//if (seb == 0) {
//if (ms_topmenu_size != 230856) {
//stop_topmenu = 1;
//}
//}

if (stop_topmenu == 0) {
				ms_read_topmenu();
				clear_midscreen();
				ms_verify_topmenu();
				nomenutm = 1;	// set status so menu isn't printed
				topmenu_size = ms_topmenu_size;
				topmenusize = ms_topmenu_size;

if (topmenusize == 247988) {
if (oea == 1) {
dect = 1;
}
}

if (topmenusize == 210836) {
if (oea == 1) {
dect = 0;
editxmb = 0;
}
}

				sam = 1; // set sample address counter
				samlock = 1; // re-enable player routine
				samsel = 4; // select flash sample
				print_message();
} // stop_topmenu
if (stop_topmenu == 1) {
				print_blankfooter();
				pspDebugScreenSetTextColor(colord);
				pspDebugScreenSetXY(18, 28);
				printf("      INCOMPATIBLE FILE!");
				for(xgt=0; xgt<190; xgt++) {
				sceDisplayWaitVblankStart();
				}
} // stop_topmenu

		}
		}
		}

} // while

}

void print_coco() {
	for(xgt=0; xgt<40; xgt++) {
	sceDisplayWaitVblankStart();
	}
	sam = 1; // set sample address counter
	samlock = 1; // re-enable player routine
	samsel = 7; // select button press sample
	pspDebugScreenSetXY(16, 12);
	printf(" Country Code: %02X",Country[61]);
}

void check_topmenu() {
	int hhh;

	hhh = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
	topmenu_size = sceIoLseek32(hhh, 0, SEEK_END);
	sceIoClose(hhh);
}

void ms_check_topmenu() {
	int hhh;

	hhh = sceIoOpen("./Xflash/Modified/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
	ms_topmenu_size = sceIoLseek32(hhh, 0, SEEK_END);
	sceIoClose(hhh);
}

void ms_read_topmenu() {
	int xtm;

	xtm = sceIoOpen("./Xflash/Modified/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
	sceIoRead(xtm,opening,ms_topmenu_size);
	sceIoClose(xtm);
}

void ms_verify_topmenu() {

	int plps;

	print_blankfooter();
	pspDebugScreenSetTextColor(colord);
	pspDebugScreenSetXY(19, 28);
	printf("   COPY: topmenu_plugin.rco");
	for(plps=0; plps<110; plps++) {
	sceDisplayWaitVblankStart();
	}

	int vap;
	int sap;
		if (test == 0) {
		sap = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		sap = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
	sceIoWrite(sap,opening,ms_topmenu_size);
	sceIoClose(sap);

	for(vap=0; vap<100; vap++) {
	sceDisplayWaitVblankStart();
	}

	int uhj;
	int fxtv;
	int tma;
		if (test == 0) {
		fxtv = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
		}
		else {
		fxtv = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_RDONLY, 0777);
		}
	sceIoRead(fxtv,vopening,ms_topmenu_size);
	sceIoClose(fxtv);

	verify = 0;
	for(tma=0; tma<ms_topmenu_size; tma++) {
		if (opening[tma] != vopening[tma]) {
		verify = 1;
		}
	}

	if (verify == 1) {
	print_verify_failed();

	if (fails < 3) {
		for(ixt=0; ixt<170; ixt++) {
		sceDisplayWaitVblankStart();
		}

		if (test == 0) {
		fdo = sceIoOpen("flash0:/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}
		else {
		fdo = sceIoOpen("./Xflash/Dump/flash0/vsh/resource/topmenu_plugin.rco", PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
		}

		sceIoWrite(fdo,opening,ms_topmenu_size);
		sceIoClose(fdo);
		pspDebugScreenSetXY(18, 28);
		printf("          AUTO RETRY           ");
		for(ixt=0; ixt<110; ixt++) {
		sceDisplayWaitVblankStart();
		}
		ms_verify_topmenu();
	}

	}

	else {
	print_blankfooter();
	finger_ok_animB();
	print_verify_passed();
	}

	for(uhj=0; uhj<60; uhj++) {
	sceDisplayWaitVblankStart();
	}
	exist = 1;
}

void fix_bmp_error() {
	int fdv;
	int i;

	if (oea == 1) {
	fdv = sceIoOpen("flash0:/vsh/resource/07.bmp", PSP_O_RDONLY, 0);

	if(fdv < 0) {
		bmperr = 0;
        }
	else {
                bmperr = 1;
        }
	sceIoClose(fdv);
	}//oea

if (bmperr == 1) {
if (oea == 1) {

	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(0, 17);
	printf("                            PLEASE WAIT ....          ");

	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/01.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/02.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/03.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/04.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/05.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/06.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/07.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/08.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/09.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/10.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/11.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	sceIoRemove("flash0:/vsh/resource/12.bmp");
	for(i=0; i<20; i++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetXY(0, 17);
	printf("                                                      ");

} // oea
} // bmperr
}

void warn_irshell() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000088); // Red for warning screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n                                                           ");
	printf("\n                                                           ");
	printf("\n             X-FLASH HAS DETECTED THAT IT WAS LAUNCHED     ");
	printf("\n            FROM WITHIN iR Shell. WHILE A GREAT PROGRAM,   ");
	printf("\n                IT IS NOT SAFE TO ALLOW MEMORY STICK       ");
	printf("\n              ACCESS BY TWO PROGRAMS AT THE SAME TIME.     ");
	printf("\n                                                           ");
	printf("\n                      PROCEED AT YOUR OWN RISK!            ");
	printf("\n                                                           ");
	printf("\n                                                           ");
	five_lines();
fix_screen();
}

void warn_fix() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000088); // Red for warning screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n                                                           ");
	printf("\n                                                           ");
	printf("\n          X-FLASH HAS FOUND BACKGROUND FILES THAT BELONG   ");
	printf("\n         ONLY IN OLDER FIRMWARE VERSIONS. THESE WERE LEFT  ");
	printf("\n       BY A BUG IN THE AUTO RESTORE ROUTINE OF X-FLASH V18 ");
	printf("\n       AND V18b. THESE FILES HAVE BEEN REMOVED BY X-FLASH. ");
	printf("\n               YOU SHOULD NOT SEE THIS MESSAGE AGAIN.      ");
	five_lines();
fix_screen();
}

void warn_registry() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000088); // Red for warning screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n                                                           ");
	printf("\n                                                           ");
	printf("\n              X-FLASH HAS DETECTED THAT YOUR REGISTRY      ");
	printf("\n           VERSION DOES NOT MATCH YOUR FIRMWARE VERSION.   ");
	printf("\n           THIS MAY BE THE RESULT OF A FIRMWARE UPGRADE    ");
	printf("\n        YOU SHOULD RESTORE THE PSP UNIT'S DEFAULT SETTINGS ");
	printf("\n        THROUGH THE PSP XMB MENU AND REENTER YOUR SETTINGS.");
	five_lines();
fix_screen();
}

void warn_flash() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00000088); // Red for warning screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n                                                           ");
	printf("\n                                                           ");
	printf("\n          X-FLASH HAS ESTIMATED AN EXTREMELY LOW AMOUNT OF ");
	printf("\n          SPACE REMAINING ON THE FLASH CHIP WHERE FIRMWARE ");
	printf("\n          FILES ARE STORED. THE COMBINATION OF CUSTOM FILES");
	printf("\n           YOU HAVE WRITTEN HAS ALMOST FILLED FLASH SPACE. ");
	printf("\n             TRY USING A SMALLER GAMEBOOT, OR FONT FILE.   ");
	five_lines();
fix_screen();
}

void fix_screen() {
	int xmp;
	for(xmp=0; xmp<999; xmp++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenInit();
	print_header();
	print_justfooter();
	clear_midscreen();
	for(xmp=0; xmp<29; xmp++) {
	sceDisplayWaitVblankStart();
	}
}

void warn_nodax() {
	int xmp;
	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x000A0AEE);
	pspDebugScreenSetBackColor(0x00000000);
	pspDebugScreenSetXY(0, 4);
	printf("                               WARNING:\n\n");
	pspDebugScreenSetTextColor(0x00EFDDCA);
	printf("                                                             \n");
	printf("          X-Flash has detected that this is it's first use,  \n");
	printf("         and this PSP unit does not have any custom firmware \n");
	printf("         files installed that would provide a recovery mode. \n");
	printf("                                                             \n");
	printf("        It is recommended that you install a custom firmware \n");
	printf("        before using X-Flash, or use X-Flash to apply the DAX\n");
	printf("         custom firmware files as your first write operation.\n");
	printf("                                                             \n");
	printf("         This will provide a recovery mode which lowers the  \n");
	printf("        risk of any permanent damage that could be caused by \n");
	printf("        writing incompatible fonts and gameboots to firmware.\n");
	printf("\n\n");
	printf("        If you press Cross at this screen, X-Flash will auto \n");
	printf("        apply the Dark_AleX custom firmware files, and then  \n");
	printf("                         continue to the menu.               \n");
	printf("          If you press Circle at this screen, X-Flash will   \n");
	printf("         continue to the menu without installing any files.  \n");
	printf("                         \n\n");
	for(xmp=0; xmp<99; xmp++) {
	sceDisplayWaitVblankStart();
	}
	pspDebugScreenSetTextColor(0x000A0AEE);
	printf("               PRESS 'X' TO INSTALL or 'O' TO CONTINUE\n\n");
	printf("      THIS SCREEN WILL NOT BE SEEN AGAIN FOR THIS INSTALLATION.");
	for(xmp=0; xmp<9; xmp++) {
	sceDisplayWaitVblankStart();
	}
}

void boot_power() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n            X-FLASH HAS DETECTED A LOW BATTERY CONDITION.  ");
	printf("\n           IT IS NOT SAFE TO CONTINUE USING FLASH WRITING  ");
	printf("\n             TOOLS WITH A VERY LOW BATTERY, AND NO MAINS   ");
	printf("\n           CABLE PLUGGED IN. X-FLASH WILL EXIT FOR SAFETY. ");
	just_boot();
}

void boot_notse() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n             VERSION 2.71 OR 3.02 FIRMWARE DETECTED BUT    ");
	printf("\n           THIS DOES NOT APPEAR TO BE AN ORIGINAL RELEASE  ");
	printf("\n              OF A SUPPORTED Dark_AleX HYBRID FIRMWARE.    ");
	printf("\n           IF YOU ARE USING 2.71 SE-A YOU SHOULD UPGRADE.  ");
	printf("\n                          X-FLASH WILL EXIT.               ");
	just_boot();
}

void boot_bootof() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n            X-FLASH HAS DETECTED A PROBLEM WITH THE FILE   ");
	printf("\n            PSP/GAME/XFLASH/Xflash/Modified/gameboot.pmf   ");
	printf("\n            IT MUST NOT BE ANY LARGER THAN 2500000 BYTES   ");
	printf("\n                    FOR PSP FIRMWARE VERSION 1.50.         ");
	printf("\n           IT MUST BE A VALID PMF FILE OR ZERO BYTE FILE.  ");
	just_boot();
}

void boot_bootse() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n            X-FLASH HAS DETECTED A PROBLEM WITH THE FILE   ");
	printf("\n            PSP/GAME/XFLASH/Xflash/Modified/gameboot.pmf   ");
	printf("\n            IT MUST NOT BE ANY LARGER THAN 618 KILOBYTES   ");
	printf("\n                    FOR PSP FIRMWARE VERSION 2.71 SE.      ");
	printf("\n           IT MUST BE A VALID PMF FILE OR ZERO BYTE FILE.  ");
	just_boot();
}

void boot_bootoe() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n            X-FLASH HAS DETECTED A PROBLEM WITH THE FILE   ");
	printf("\n            PSP/GAME/XFLASH/Xflash/Modified/gameboot.pmf   ");
	printf("\n            IT MUST NOT BE ANY LARGER THAN 196 KILOBYTES   ");
	printf("\n                    FOR PSP FIRMWARE VERSION 3.02 OE.      ");
	printf("\n           IT MUST BE A VALID PMF FILE OR ZERO BYTE FILE.  ");
	just_boot();
}

void boot_music() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n             X-FLASH HAS DETECTED AN INVALID OR MISSING    ");
	printf("\n                        MUSIC FILE  'sndcx.XF'             ");
	printf("\n            THIS VERSION IS TESTED WITH THE FILE PRESENT   ");
	printf("\n               YOU MUST HAVE THE FILE PRESENT TO START     ");
	just_boot();
}

void boot_vag() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n             X-FLASH HAS DETECTED AN INVALID OR MISSING    ");
	printf("\n              VAG FILE  (Xflash/Modified/Coldboot.vag)     ");
	printf("\n             OR SOURCE FILE WAS NOT NAMED 'Coldboot.wav'   ");
	printf("\n              YOU MUST HAVE A VAG FILE PRESENT TO START    ");
	just_boot();
}

void boot_upd() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n            YOU MUST USE THE PREVIOUS VERSION OF X-FLASH   ");
	printf("\n            TO RESTORE NETWORK UPDATE ICON FUNCTIONALITY   ");
	printf("\n             OTHERWISE THIS VERSION WILL NOT BE ABLE TO    ");
	printf("\n                RESTORE NETWORK UPDATE FUNCTIONALITY       ");
	just_boot();
}

void boot_ltn() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n           YOU MUST ONLY USE VALID PGF FONTS WITH X-FLASH  ");
	printf("\n            ANY NEW FONTS YOU INTRODUCE SHOULD BE TESTED   ");
	printf("\n                WITH THE AID OF A FIRMWARE LAUNCHER        ");
	printf("\n          X-FLASH HAS DETECTED AN INVALID OR MISSING FONT  ");
	printf("\n             OR THE FILE IS GREATER THAN 1000000 BYTES.    ");
	just_boot();
}

void boot_signature() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n               X-FLASH HAS DETECTED A CHANGE TO BUILT      ");
	printf("\n          IN FILES THAT ARE CONTAINED WITHIN THE PROGRAM   ");
	printf("\n          THIS WOULD BYPASS THE STARTUP CHECKING OF FILES  ");
	printf("\n           THAT IS DONE BY THE PROGRAM FOR EVERY SESSION   ");
	printf("\n                TRY USING A CLEAN INSTALL OF X-FLASH       ");
	just_boot();
}

void boot_irshell() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n             X-FLASH HAS DETECTED THAT IT WAS LAUNCHED     ");
	printf("\n            FROM WITHIN iR Shell. WHILE A GREAT PROGRAM,   ");
	printf("\n           IT IS NOT SAFE TO ALLOW POSSIBLE MEMORY STICK   ");
	printf("\n          ACCESS BY TWO PROGRAMS AT THE SAME TIME. LAUNCH  ");
	printf("\n             USING THE KXPLOIT METHOD OR NOKXPLOIT HACK    ");
	just_boot();
}

void boot_bmp() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n         YOU MUST ONLY USE VALID BACKGROUNDS WITH X-FLASH  ");
	printf("\n        ANY NEW BACKGROUNDS YOU INTRODUCE SHOULD BE TESTED ");
	printf("\n       WITH A FIRMWARE LAUNCHER AND NO LARGER THAN 480x272 ");
	printf("\n X-FLASH HAS DETECTED AN INVALID OR MISSING BACKGROUND FILE");
	just_boot();
}

void boot_bmx() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n         YOU MUST ONLY USE VALID BACKGROUNDS WITH X-FLASH  ");
	printf("\n          ANY NEW BACKGROUNDS YOU INTRODUCE FOR 3.02 OE    ");
	printf("\n        FIRMWARE VERSION MUST BE EXACTLY 60x34 RESOLUTION. ");
	printf("\n        X-FLASH HAS DETECTED AN INCORRECT BACKGROUND FILE. ");
	just_boot();
}

void boot_png() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n        YOU MUST USE A VALID PNG AS THE NAME PLATE PICTURE ");
	printf("\n       ANY NEW PNG PICTURES YOU INTRODUCE SHOULD BE TESTED ");
	printf("\n      WITH A FIRMWARE LAUNCHER AND SHOULD BE SIZED 480x272 ");
	printf("\n      X-FLASH HAS DETECTED AN INVALID OR MISSING PNG FILE  ");
	just_boot();
}

void boot_security() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n                                                           ");
	printf("\n         PARENTAL CONTROL MUST BE TURNED OFF FOR X_FLASH   ");
	printf("\n         TO START. THIS IS A SECURITY FEATURE TO INHIBIT   ");
	printf("\n         UNAUTHORIZED USE. TRY WITH PARENTAL CONTROL OFF.  ");
	printf("\n                                                           ");
	just_boot();
}

void boot_usb() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n             X-FLASH HAS DETECTED USB MODE IS ACTIVATED    ");
	printf("\n             PROGRAM LAUNCH IS BEING PREVENTED BECAUSE     ");
	printf("\n             USB MODE IS KNOWN TO CAUSE FILE CORRUPTION    ");
	printf("\n               WHEN USED SIMULTANEOUSLY WITH FILE I/O      ");
	printf("\n          STARTING X-FLASH FROM A SHELL IS NOT RECOMMENDED ");
	just_boot();
}

void boot_mem() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n             X-FLASH HAS DETECTED AN ACTIVE APPLICATION    ");
	printf("\n             THAT IS CAUSING UNUSUAL MEMORY CONSUMPTION    ");
	printf("\n             X-FLASH MAY HAVE BEEN LAUNCHED FROM A SHELL   ");
	printf("\n              TRY LAUNCHING X-FLASH FROM THE GAME MENU     ");
	printf("\n             USING THE KXPLOIT METHOD OR NOKXPLOIT HACK    ");
	just_boot();
}

void boot_mikmem() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n              X-FLASH HAS DETECTED A LOW AVAILABLE RAM     ");
	printf("\n             CONDITION CAUSED BY THE MUSIC MODULE BEING    ");
	printf("\n             LOADED TOO MANY TIMES FOR THE CREDITS DEMO    ");
	printf("\n            OR THE FREE SPACE GAME. X-FLASH WILL EXIT IN   ");
	printf("\n           ORDER TO PREVENT A CRITICAL LOW RAM CONDITION.  ");
	just_boot();
}

void boot_labels() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n          X-FLASH HAS FOUND A LABELS.XF FILE OF INCORRECT  ");
	printf("\n         SIZE, OR INCORRECT FORMAT. PLEASE USE A LABELS.XF ");
	printf("\n          FILE CREATED EITHER WITH X-FLASH, OR THE WINDOWS ");
	printf("\n           SUPPLEMENTAL LABEL EDITOR WRITTEN FOR X-FLASH.  ");
	printf("\n                                                           ");
	just_boot();
}

void boot_labelsum() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n         X-FLASH HAS FOUND A LABELS.XF FILE WITH INCORRECT ");
	printf("\n          CHECKSUM OR FILE FORMAT. PLEASE USE A LABELS.XF  ");
	printf("\n                FILE CREATED WITH HELLCAT'S WINDOWS        ");
	printf("\n               SUPPLEMENTAL LABEL EDITOR FOR X-FLASH.      ");
	printf("\n                                                           ");
	just_boot();
}

void boot_prelim() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n           X-FLASH HAS NOT FOUND THE CORRECT NICKNAME ON   ");
	printf("\n         THIS PSP FOR PRELIMINARY COPIES OF X-FLASH TO RUN ");
	printf("\n           PLEASE ENTER THE CORRECT NICKNAME FROM THE XMB  ");
	printf("\n            MENU, OR DOWNLOAD THE LATEST PUBLIC RELEASE.   ");
	printf("\n                                                           ");
	just_boot();
}

void boot_theme() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n           X-FLASH HAS FOUND A THEME.XF FILE OF INCORRECT  ");
	printf("\n         SIZE, OR INCORRECT FORMAT. PLEASE USE THE CORRECT ");
	printf("\n         THEME.XF FILE CREATED BY X-FLASH AT PROGRAM START,");
	printf("\n                 OR WHEN A MENU COLOUR THEME IS SAVED.     ");
	printf("\n                                                           ");
	just_boot();
}

void boot_topmenu() {
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 5);
	printf("\n        X-FLASH HAS FOUND A FIRMWARE FILE OF THE INCORRECT ");
	printf("\n         SIZE FOR THIS FIRMWARE VERSION. THE FIRMWARE FILE ");
	printf("\n                TOPMENU_PLUGIN.RCO MIGHT BE CORRUPT.       ");
	printf("\n             YOU SHOULD TRY REINSTALLING YOUR FIRMWARE,    ");
	printf("\n             OR COPY THE VALID FIRMWARE FILE TO FLASH.     ");
	just_boot();
}

void print_boot() {
	int bs;
 	pspDebugScreenInit();
	pspDebugScreenSetTextColor(0x00EFDDCA);
	pspDebugScreenSetBackColor(0x00880000); // Blue for error screen
	pspDebugScreenSetXY(0, 7);
	printf("\n                                                       ");
	printf("\n                                                       ");
	printf("\n               .-'\\                                   ");
	printf("\n            .-'  `/\\                                  ");
	printf("\n         .-'      `/\\      X-FLASH IS FOR VERSION     ");
	printf("\n         \\         `/\\   1.50 OR 2.71 SE FIRMWARE    ");
	printf("\n          \\         `/\\   VERSION PSP UNITS ONLY.    ");
	printf("\n           \\    _-   `/\\       _.--.                 ");
	printf("\n            \\    _-   `/`-..--\\     )                ");
	printf("\n             \\    _-   `,','  /    ,')                ");
	printf("\n              `-_   -   ` -- ~   ,','                  ");
	printf("\n               `-              ,','                    ");
	printf("\n                \\,--.    ____==-~                     ");
 	printf("\n                 \\   \\_-~\\                          ");
	printf("\n                  `_-~_.-' YOUR PSP GETS THE BOOT!     ");
	printf("\n                   \\-~             SEE YA!   :D       ");
	printf("\n                                                       ");
	printf("\n                                                       ");
	pspAudioEnd();
	for(bs=0; bs<999; bs++) {
	sceDisplayWaitVblankStart();
	}
}

void just_boot() {
	int xmp;
	printf("\n                                                           ");
	printf("\n                   .-'\\                                   ");
	printf("\n                .-'  `/\\                                  ");
	printf("\n             .-'      `/\\                                 ");
	printf("\n             \\         `/\\                               ");
	printf("\n              \\         `/\\                              ");
	printf("\n               \\    _-   `/\\       _.--.                 ");
	printf("\n                \\    _-   `/`-..--\\     )                ");
	printf("\n                 \\    _-   `,','  /    ,')                ");
	printf("\n                  `-_   -   ` -- ~   ,','                  ");
	printf("\n                   `-              ,','                    ");
	printf("\n                    \\,--.    ____==-~                     ");
 	printf("\n                     \\   \\_-~\\                          ");
	printf("\n                      `_-~_.-'     YOU GET THE BOOT!       ");
	printf("\n                       \\-~             SEE YA!   :D       ");
	printf("\n                                                           ");
	printf("\n                                                           ");
	pspAudioEnd();
	for(xmp=0; xmp<999; xmp++) {
	sceDisplayWaitVblankStart();
	}
}

void find_sig() {
calcsig = 0;		// find ram signature
sigset = 0x70 + 0x06;	// set ram signature
}

void set_program_version() { // ><-Flash V1Xx   XX/XX/07
ver[0] = 0x3E; // >
ver[1] = 0x3C; // <
ver[2] = 0x2D; // -
ver[3] = 0x46; // F
ver[4] = 0x6C; // l
ver[5] = 0x61; // a
ver[6] = 0x73; // s
ver[7] = 0x68; // h
ver[8] = 0x20; //  
ver[9] = 0x56; // V
ver[10] = 0x31; // 1
ver[11] = 0x38; // 8
ver[12] = 0x67; // g
ver[13] = 0x20; //  
ver[15] = 0x20; //  
ver[16] = 0x20; // 
ver[17] = 0x30; // 0
ver[18] = 0x31; // 1
ver[19] = 0x2F; // /
ver[20] = 0x30; // 0
ver[21] = 0x31; // 1
ver[22] = 0x2F; // /
ver[23] = 0x30; // 0
ver[24] = 0x37; // 7
}

