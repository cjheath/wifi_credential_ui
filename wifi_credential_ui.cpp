/*
 * LVGL user interface program to scan and display Wifi APs, select one, and enter+manage passwords
 */
#include	"ui_task.h"
#include	"lvgl.h"
#include	"wifi_credential_ui.h"

static	lv_obj_t*	wifi_scan_gui;
static	lv_obj_t*	keyboard_ui;

#define	TOP_HEIGHT	30	// Room for the title
#define	BOTTOM_HEIGHT	30	// Room for the ok/cancel buttons

static	void	show_wifi_credential_ui(bool show);
static	void	makeKeyboard();

void create_wifi_credential_ui(void)
{
	lv_obj_t*	wifi_scan_list;
	lv_obj_t* cancel_btn;

	/*
	 * Get the Display and screen
	 */
	lv_disp_t*	display = lv_disp_get_default();
	int screenWidth = display->driver->hor_res;
	int screenHeight = display->driver->ver_res;
	lv_obj_t*	screen = lv_scr_act();

	wifi_scan_gui = lv_obj_create(screen);
	lv_obj_remove_style(wifi_scan_gui, NULL, LV_PART_ANY | LV_STATE_ANY);
	lv_obj_set_pos(wifi_scan_gui, 0, 0);
	lv_obj_set_style_bg_color(wifi_scan_gui, lv_color_black(), 0);
	lv_obj_set_size(wifi_scan_gui, screenWidth, screenHeight);

	lv_obj_t*	wifi_scan_header = lv_label_create(wifi_scan_gui);
	// lv_obj_remove_style(wifi_scan_header, NULL, LV_PART_ANY | LV_STATE_ANY);
	lv_obj_set_pos(wifi_scan_header, 0, 0);
	lv_obj_set_size(wifi_scan_header, screenWidth, TOP_HEIGHT);
	lv_obj_set_align(wifi_scan_header, LV_ALIGN_TOP_MID);
	lv_label_set_text(wifi_scan_header, "Scanning for WiFi APs...");
	lv_obj_set_style_text_font(wifi_scan_header, &lv_font_montserrat_24, 0);

	wifi_scan_list = lv_list_create(wifi_scan_gui);
	// lv_obj_remove_style(wifi_scan_list, NULL, LV_PART_ANY | LV_STATE_ANY);
	lv_obj_set_pos(wifi_scan_list, 0, TOP_HEIGHT);
	lv_obj_set_size(wifi_scan_list, screenWidth, screenHeight-TOP_HEIGHT-BOTTOM_HEIGHT);
	// lv_obj_align(wifi_scan_list, LV_ALIGN_CENTER, 0, -20);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_width(&style, 12);		 /* Width of the scrollbar */
	lv_obj_add_style(wifi_scan_list, &style, LV_PART_SCROLLBAR);

	static char btn_text[] = "Button x";
	for (int i = 'A'; i < 'Z'; i++)
	{
		btn_text[7] = i;
		lv_obj_t* b = lv_list_add_btn(
				wifi_scan_list,
				(i-'A')%2 == 0 ? LV_SYMBOL_EYE_OPEN : LV_SYMBOL_EYE_CLOSE,
				btn_text
			);
	}

	cancel_btn = lv_btn_create(wifi_scan_gui);
	// lv_obj_remove_style(cancel_btn, NULL, LV_PART_ANY | LV_STATE_ANY);
	lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
	lv_obj_add_event_cb(cancel_btn,
		[](lv_event_t* event){
			if (event->code == LV_EVENT_CLICKED) show_wifi_credential_ui(false);
		},
		LV_EVENT_CLICKED, NULL
	);
	lv_obj_t* label = lv_label_create(cancel_btn);
	lv_label_set_text(label, "Cancel");
	lv_obj_center(label);

#if 0
	makePW();
	lv_ex_btnmatrix_1();

	bg_wifiota = lv_obj_create(lv_scr_act(), NULL);
	lv_obj_remove_style(bg_wifiota, NULL, LV_PART_ANY | LV_STATE_ANY);
	// lv_obj_set_style_local_bg_opa(bg_wifiota, LV_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
	// lv_obj_set_style_local_bg_color(bg_wifiota, LV_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_obj_set_pos(bg_wifiota, 0, topHeight);
	lv_obj_set_size(bg_wifiota, LV_HOR_RES, screenHeight - topHeight - bottomHeight);

	lv_obj_move_background(wifi_scan_gui);

	static lv_style_t text_style;
	lv_style_init(&text_style);

	/*Set a background color and a radius*/
	lv_style_set_radius(&text_style, LV_STATE_DEFAULT, 5);
	lv_style_set_bg_opa(&text_style, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&text_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_value_align(&text_style, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
	lv_style_set_text_font(&text_style, LV_STATE_DEFAULT, &lv_font_montserrat_18);

	ota_label = lv_label_create(bg_wifiota, NULL);
	lv_label_set_text(ota_label, "");
	lv_obj_add_style(ota_label, LV_PART_MAIN, &text_style);
	lv_obj_align(ota_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_move_background(bg_wifiota);

	ota_label1 = lv_label_create(bg_wifiota, NULL);
	lv_obj_add_style(ota_label1, LV_PART_MAIN, &text_style);
	lv_label_set_text(ota_label1, "");
	lv_obj_align(ota_label1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 30);
	lv_obj_move_background(bg_wifiota);

	wifi_group = lv_group_create();
	lv_indev_set_group(encoder_indev_t, wifi_group);

	lv_group_add_obj(wifi_group, wifi_scan_list);
	lv_group_add_obj(wifi_group, cancel_btn);
#endif
}

static void show_wifi_credential_ui(bool show)
{
	if (show)
	{
		lv_obj_move_foreground(wifi_scan_gui);
		/*
		lv_indev_set_group(encoder_indev_t, wifi_group);
		lv_obj_set_hidden(wifi_scan_list, false);
		lv_obj_set_hidden(cancel_btn, false);
		xTaskCreate(wifi_scan_network,"wifi_scan",4096,NULL,1,&hWifiTask);
		*/
	}
}

void create_wifi_credential_button(lv_obj_t* parent)
{
#if 0
	/*
	 * Make a button to open the WiFi gui
	 */
	lvobj*	wifi_button = lv_btn_create(parent, NULL);
	lv_btn_set_checkable(wifi_button, true);
	lv_btn_toggle(wifi_button);
	lv_obj_set_size(wifi_button, 40, 20);
	lv_obj_align(wifi_button, NULL, LV_ALIGN_CENTER, 170, 10);

	lv_obj_t* label = lv_label_create(wifi_button, NULL);
	lv_label_set_text(label, "Wifi");

	lv_obj_add_event_cb(wifi_button, []() {
		show_wifi_credential_ui(true);
	});

	return wifi_button;
#endif
}

static void keyboard_event_cb(lv_event_t* event)
{
	lv_keyboard_def_event_cb(event);

	if (event->code == LV_EVENT_READY) {
		lv_obj_move_background(keyboard_ui);
/*		const char* password = String(lv_textarea_get_text(ta_password));
		lv_obj_set_hidden(ta_password, true);
		lv_obj_set_hidden(pwd_label, true);
		lv_obj_set_hidden(btnm1, false);
		lv_obj_set_hidden(pwd_label1, false);
*/
	}
	else if (event->code == LV_EVENT_CANCEL)
	{
		lv_obj_move_background(keyboard_ui);
	}
}

static void makeKeyboard()
{
	keyboard_ui = lv_keyboard_create(lv_scr_act());
	lv_obj_set_size(keyboard_ui, LV_HOR_RES, LV_VER_RES / 2);
	// lv_keyboard_set_cursor_manage(keyboard_ui, true);

	// lv_keyboard_set_textarea(keyboard_ui, ta_password);
	lv_obj_add_event_cb(keyboard_ui, keyboard_event_cb, LV_EVENT_ALL, 0);
	lv_obj_move_background(keyboard_ui);
}
