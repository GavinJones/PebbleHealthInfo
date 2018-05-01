#include "pebble.h"

#define SETTINGS_KEY 1

Window *window;

static TextLayer *sleep_layer, *active_layer, *calorie_layer, *distance_layer, *step_layer, *stepcolour_layer;
//                 *Xsleep_layer, *Xactive_layer, *Xcalorie_layer, *Xdistance_layer;

static GFont textFont, numFont, bigFont;

// A structure containing our settings
typedef struct ClaySettings {
  int StepGoal;
  int stepcolour;
  
} __attribute__((__packed__)) ClaySettings;

// A struct for our specific settings (see main.h)
ClaySettings settings;

// Initialize the default settings
static void prv_default_settings() {
  settings.StepGoal = 4000;
  settings.stepcolour = 0x0000FF;
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  // update_time();
}



void click_handler(ClickRecognizerRef recognizer, void *context) {
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "Single Click");

  // if (persist_read_bool(TEXT_HIDDEN_KEY)) {
  //   persist_write_bool(TEXT_HIDDEN_KEY, false);
  //   battery_state_receiver(battery_state_service_peek());
  // } else {
  //   persist_write_bool(TEXT_HIDDEN_KEY, true);
  //   text_layer_set_text(charge_status, "");
 // }
}

void long_click_handler(ClickRecognizerRef recognizer, void *context) {

  // if (persist_read_bool(ENABLE_COLOR_KEY)) {
  //   persist_write_bool(ENABLE_COLOR_KEY, false);
  //   battery_state_receiver(battery_state_service_peek());
  // } else {
  //   persist_write_bool(ENABLE_COLOR_KEY, true);
  //   battery_state_receiver(battery_state_service_peek());
  // }

}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, click_handler);
}

static void health_handler(HealthEventType event, void *context) {

 static char s_value_buffer[38];
 long sec, hr, min, t; 
  
  // display the step count
  snprintf(s_value_buffer, sizeof(s_value_buffer), "%d", (int)health_service_sum_today(HealthMetricStepCount));
  text_layer_set_text(step_layer, s_value_buffer);
  
  int step_count = atoi(s_value_buffer);
  if(step_count > settings.StepGoal) {
    PBL_IF_COLOR_ELSE(text_layer_set_text_color(step_layer, GColorGreen),text_layer_set_text_color(step_layer, GColorWhite));
    } 
  
  static char a_value_buffer[38];
  
  sec = (int)health_service_sum_today(HealthMetricActiveSeconds);
  hr = sec/3600;
  t = sec%3600;
  min = t/60;
  sec = t%60;
  snprintf(a_value_buffer, sizeof(a_value_buffer), "%ldh %ldm", hr, min);
  
  text_layer_set_text(active_layer, a_value_buffer);
  
  
  static char d_value_buffer[38];
  static float num_miles;
  num_miles = (int)health_service_sum_today(HealthMetricWalkedDistanceMeters) / 1609.344;
  
  static char d_short_buffer[3];
  snprintf(d_short_buffer, sizeof(d_short_buffer), "%d", (int)(num_miles*1000)%1000);
  snprintf(d_value_buffer, sizeof(d_value_buffer), "%d.%sm", (int)num_miles, d_short_buffer);
  
  text_layer_set_text(distance_layer, d_value_buffer);
  
  static char k_value_buffer[38];
    
  snprintf(k_value_buffer, sizeof(k_value_buffer), "%dk", (int)health_service_sum_today(HealthMetricRestingKCalories) + (int)health_service_sum_today(HealthMetricActiveKCalories));
  text_layer_set_text(calorie_layer, k_value_buffer);

  static char z_value_buffer[38];
  
  sec = (int)health_service_sum_today(HealthMetricSleepSeconds);
  hr = sec/3600;
  t = sec%3600;
  min = t/60;
  sec = t%60;
  snprintf(z_value_buffer, sizeof(z_value_buffer), "%ldh %ldm", hr, min);
    
  // snprintf(z_value_buffer, sizeof(z_value_buffer), "Zzz: %dm", (int)health_service_sum_today(HealthMetricSleepSeconds) / 60);
  text_layer_set_text(sleep_layer, z_value_buffer);
  
}  

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *tpstepgoal = dict_find(iter, MESSAGE_KEY_stepgoal);
  if(tpstepgoal) {
    settings.StepGoal = tpstepgoal->value->int32;

  // Save the new settings to persistent storage
  prv_save_settings();
  }
    
  Tuple *stepcolour = dict_find(iter, MESSAGE_KEY_stepcolour);
  if(stepcolour) {
    settings.stepcolour = stepcolour->value->int32;

  // Save the new settings to persistent storage
  prv_save_settings();
  }
  
}

void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  
  // GRect bounds = layer_get_bounds(window_layer);

  window_set_background_color(window, GColorBlack);
  
  // Load custom fonts
    
  textFont = PBL_IF_ROUND_ELSE(fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FULL_44)), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FULL_40)));

  numFont = PBL_IF_ROUND_ELSE(fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PIXEL_23)), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PIXEL_31)));

  bigFont = PBL_IF_ROUND_ELSE(fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PIXEL_31)), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_PIXEL_31)));


  // textFont = fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS);
  // numFont = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  
  // Create Active Minutes TextLayer
  active_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 58, 180, 40)), text_layer_create(GRect(1, -7, 144, 40)));
  text_layer_set_background_color(active_layer, GColorClear);
  text_layer_set_text_color(active_layer, GColorWhite);
  text_layer_set_font(active_layer, bigFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(active_layer, GTextAlignmentCenter),text_layer_set_text_alignment(active_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(active_layer));

 // Create Distance TextLayer
  distance_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 13, 180, 40)), text_layer_create(GRect(1, 27, 144, 40)));
  text_layer_set_background_color(distance_layer, GColorClear);
  text_layer_set_text_color(distance_layer, GColorWhite);
  text_layer_set_font(distance_layer, numFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(distance_layer, GTextAlignmentCenter),text_layer_set_text_alignment(distance_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(distance_layer));
    
 // Create Calorie TextLayer
  calorie_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 32, 180, 40)), text_layer_create(GRect(1, 61, 144, 40)));
  text_layer_set_background_color(calorie_layer, GColorClear);
  text_layer_set_text_color(calorie_layer, GColorWhite);
  text_layer_set_font(calorie_layer, bigFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(calorie_layer, GTextAlignmentCenter),text_layer_set_text_alignment(calorie_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(calorie_layer));

  // Create Step Colour TextLayer
  stepcolour_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 95, 180, 40)), text_layer_create(GRect(0, 0, 0, 0)));
  text_layer_set_background_color(stepcolour_layer, GColorBlack);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(stepcolour_layer));
  
  // Create Step TextLayer
  step_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 85, 180, 53)), text_layer_create(GRect(1, 90, 144, 54)));
  text_layer_set_background_color(step_layer, GColorClear);
  PBL_IF_COLOR_ELSE(text_layer_set_text_color(step_layer, GColorFromHEX(settings.stepcolour)),text_layer_set_text_color(step_layer, GColorWhite));
  text_layer_set_font(step_layer, textFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(step_layer, GTextAlignmentCenter),text_layer_set_text_alignment(step_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(step_layer));
  
  // Create Sleep TextLayer
  sleep_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 132, 180, 38)),text_layer_create(GRect(1, 130, 144, 40)));  
  text_layer_set_background_color(sleep_layer, GColorClear);
  text_layer_set_text_color(sleep_layer, GColorWhite);
  text_layer_set_font(sleep_layer, numFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(sleep_layer, GTextAlignmentCenter),text_layer_set_text_alignment(sleep_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(sleep_layer));
  
  // subscribe to health events
  if(health_service_events_subscribe(health_handler, NULL)) {
    // force initial steps display
    health_handler(HealthEventMovementUpdate, NULL);
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
  } 
}

void window_unload(Window *window) {
  text_layer_destroy(sleep_layer);
  text_layer_destroy(active_layer);
  text_layer_destroy(calorie_layer);
  text_layer_destroy(distance_layer);
  text_layer_destroy(step_layer);
  text_layer_destroy(stepcolour_layer);
  text_layer_destroy(active_layer);
   
  fonts_unload_custom_font(textFont);
  fonts_unload_custom_font(numFont);
  fonts_unload_custom_font(bigFont);
  
}

void init(void) {
  prv_load_settings();
  app_message_register_inbox_received((AppMessageInboxReceived) inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());


  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  window_set_click_config_provider(window, click_config_provider);
}

void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
