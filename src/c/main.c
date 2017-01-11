#include "pebble.h"

Window *window;

static TextLayer *sleep_layer, *active_layer, *calorie_layer, *distance_layer, *step_layer;
//                 *Xsleep_layer, *Xactive_layer, *Xcalorie_layer, *Xdistance_layer, *Xstep_layer;

static GFont textFont, numFont;

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
  
  static char a_value_buffer[38];
  
  sec = (int)health_service_sum_today(HealthMetricActiveSeconds);
  hr = sec/3600;
  t = sec%3600;
  min = t/60;
  sec = t%60;
  snprintf(a_value_buffer, sizeof(a_value_buffer), "Act: %ldh %ldm", hr, min);
    
  text_layer_set_text(active_layer, a_value_buffer);
  
  static char d_value_buffer[38];
  static float num_miles;
  num_miles = (int)health_service_sum_today(HealthMetricWalkedDistanceMeters) / 1609.344;
  
  static char d_short_buffer[3];
  snprintf(d_short_buffer, sizeof(d_short_buffer), "%d", (int)(num_miles*1000)%1000);
  snprintf(d_value_buffer, sizeof(d_value_buffer), "Dis: %d.%s", (int)num_miles, d_short_buffer);
  
  text_layer_set_text(distance_layer, d_value_buffer);
  
  static char k_value_buffer[38];
    
  snprintf(k_value_buffer, sizeof(k_value_buffer), "Cals: %d", (int)health_service_sum_today(HealthMetricRestingKCalories) + (int)health_service_sum_today(HealthMetricActiveKCalories));
  text_layer_set_text(calorie_layer, k_value_buffer);

  static char z_value_buffer[38];
  
  sec = (int)health_service_sum_today(HealthMetricSleepSeconds);
  hr = sec/3600;
  t = sec%3600;
  min = t/60;
  sec = t%60;
  snprintf(z_value_buffer, sizeof(z_value_buffer), "Slp: %ldh %ldm", hr, min);
    
  // snprintf(z_value_buffer, sizeof(z_value_buffer), "Zzz: %dm", (int)health_service_sum_today(HealthMetricSleepSeconds) / 60);
  text_layer_set_text(sleep_layer, z_value_buffer);
  
}  

void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  
  // GRect bounds = layer_get_bounds(window_layer);

  window_set_background_color(window, GColorBlack);
  
  // Load custom fonts
    
  textFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SQUARE_26));
  
  numFont = PBL_IF_ROUND_ELSE(fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SQUARE_25)), fonts_load_custom_font(resource_get_handle(RESOURCE_ID_SQUARE_25)));

  // textFont = fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS);
  // numFont = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);

  // Create Step TextLayer
  step_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 3, 180, 33)), text_layer_create(GRect(1, 1, 144, 38)));
  text_layer_set_background_color(step_layer, GColorBlue);
  text_layer_set_text_color(step_layer, GColorWhite);
  text_layer_set_font(step_layer, textFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(step_layer, GTextAlignmentCenter),text_layer_set_text_alignment(step_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(step_layer));
  
  // Create Active Minutes TextLayer
  active_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 37, 180, 38)), text_layer_create(GRect(1, 32, 144, 38)));
  text_layer_set_background_color(active_layer, GColorClear);
  text_layer_set_text_color(active_layer, GColorWhite);
  text_layer_set_font(active_layer, numFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(active_layer, GTextAlignmentCenter),text_layer_set_text_alignment(active_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(active_layer));
  
 // Create Distance TextLayer
  distance_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 129, 180, 40)), text_layer_create(GRect(1, 64, 144, 38)));
  text_layer_set_background_color(distance_layer, GColorClear);
  text_layer_set_text_color(distance_layer, GColorWhite);
  text_layer_set_font(distance_layer, numFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(distance_layer, GTextAlignmentCenter),text_layer_set_text_alignment(distance_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(distance_layer));
   
 // Create Calorie TextLayer
  calorie_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 98, 180, 38)), text_layer_create(GRect(1, 96, 144, 38)));
  text_layer_set_background_color(calorie_layer, GColorClear);
  text_layer_set_text_color(calorie_layer, GColorWhite);
  text_layer_set_font(calorie_layer, numFont);
  PBL_IF_ROUND_ELSE(text_layer_set_text_alignment(calorie_layer, GTextAlignmentCenter),text_layer_set_text_alignment(calorie_layer, GTextAlignmentRight));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(calorie_layer));
 
  // Create Sleep TextLayer
  sleep_layer = PBL_IF_ROUND_ELSE(text_layer_create(GRect(1, 67, 180, 38)),text_layer_create(GRect(1, 129, 144, 40)));  
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
  text_layer_destroy(active_layer);
 
  fonts_unload_custom_font(textFont);
  fonts_unload_custom_font(numFont);
  
}

void init(void) {
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
