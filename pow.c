#include<stdio.h>
#include<stdlib.h>
#include<allegro5/allegro5.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>

/*to do:
 1. add sound effects;
 2. add a music menu where when you're hovering over an option it glows and you can obviosly click the options
 3. add a way to be defeated;
 4. make (school) shooting look better and make it possible to have more projectiles.
 */

void pls_init(bool test, const char *description){
	if(test) return;

	printf("couldn't initialize %s\n", description);
	exit(1);
}

int x, y;
float dx, dy;
int score = 0;
int e = 0;
int f = 0;
int d = 0;
int shotnum = 0;
int reqsh = 1;
int xship = 750;
int n = 0;
int yship = 750;
int yshot[10000];
int yenemypos[10000];
int xenemypos[5] = {150, 400, 650, 900, 1150};
int pos;
int m = 10;

int main(){
	pls_init(al_init(), "allegro");
	pls_init(al_install_keyboard(), "keyboard");
	pls_init(al_install_mouse(), "mouse");
	pls_init(al_install_audio(), "audio");
	pls_init(al_init_acodec_addon(), "audio codecs");
	pls_init(al_reserve_samples(16), "reserve samples");
	pls_init(al_init_image_addon(), "image addon");
	pls_init(al_init_primitives_addon(), "primitives");

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 45.0);
	pls_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	pls_init(queue, "queue");

	ALLEGRO_SAMPLE* plosion = al_load_sample("enemy-down.ogg");
	pls_init(plosion, "explosion sound");

	ALLEGRO_DISPLAY* disp = al_create_display(1500, 850);
	pls_init(disp, "display");

	ALLEGRO_FONT* font = al_create_builtin_font();
	pls_init(font, "font");

	ALLEGRO_BITMAP* spaceship = al_load_bitmap("humanship2.png");
	pls_init(spaceship, "spaceship");

	ALLEGRO_BITMAP* shot = al_load_bitmap("shot.png");
	pls_init(shot, "fire");

	ALLEGRO_BITMAP* alienship1 = al_load_bitmap("spaceship3.png");
	pls_init(alienship1, "alien ship 1");

	ALLEGRO_BITMAP* explosion = al_load_bitmap("explosion.png");
	pls_init(explosion, "explosion");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	bool done = false;
	bool introdone = false;
	bool redraw = true;
	int step = 0;
	ALLEGRO_EVENT event;

	#define KEY_SEEN     1
	#define KEY_RELEASED 2

	unsigned char key[ALLEGRO_KEY_MAX];
	memset(key, 0, sizeof(key));

	al_hide_mouse_cursor(disp);
	al_grab_mouse(disp);
	al_start_timer(timer);
	while(!introdone){
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
				if(!(yshot[n]<669) && step > 1)
					pos = xship;
		 		if(key[ALLEGRO_KEY_LEFT] && step > 0){
					xship-=10;
				        if(xship<0)
						xship +=10;
				}
				if(key[ALLEGRO_KEY_RIGHT] && step > 0){
					xship+=10;
				        if(xship>1435)
						xship -=10;							
				}

				if(key[ALLEGRO_KEY_ESCAPE] && step > 0){
					introdone = true;
					done = true;
				}
				for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
					key[i] &= KEY_SEEN;
				if (step > 1)
					yshot[n]-=10;

				redraw = true;
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
				x = event.mouse.x;
				y = event.mouse.y;
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if((event.keyboard.keycode == ALLEGRO_KEY_SPACE) && step>1)
					shotnum++;
				if((event.keyboard.keycode == ALLEGRO_KEY_ENTER)){
					step++;
					if(step == 4)
						introdone = true;
				}
				key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				key[event.keyboard.keycode] &= KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				introdone = true;
				done = true;
				break;
			case ALLEGRO_EVENT_KEY_CHAR:
				break;
		}
	
		if(redraw && al_is_event_queue_empty(queue)){	
			al_clear_to_color(al_map_rgb(0, 0, 0));
	                al_draw_scaled_bitmap(spaceship, 0, 0, al_get_bitmap_width(spaceship), al_get_bitmap_height(spaceship),
			xship, yship, al_get_bitmap_width(spaceship) / 2, al_get_bitmap_width(spaceship) / 2, 0);    
			if (step>2){
				for(int i = 0; i<5; i++){
					al_draw_bitmap(alienship1, xenemypos[i], yenemypos[m-5], 0);
					if(d == 0)
						xenemypos[i] += 10;
					if(xenemypos[4] == 1400 || xenemypos[3] == 1150 || xenemypos[2] == 900 || xenemypos[1] == 650 || xenemypos[0] == 400)
						d = 1;
					if(xenemypos[4] == 1100 || xenemypos[3] == 750 || xenemypos[2] == 500 || xenemypos[1] == 250 || xenemypos[0] == 0)
						d = 0;
					if(d == 1)
						xenemypos[i] -= 20;
				}
				if((yenemypos[m]==yshot[n])){
					if ((pos > xenemypos[0]-40)&&(!(pos > xenemypos[0]+40))){
						al_draw_bitmap(explosion, xenemypos[0], yenemypos[m-5], 0);
						f++;
						e = 0;
					}
					else if ((pos > xenemypos[1]-40)&&(!(pos > xenemypos[1]+40))){
						al_draw_bitmap(explosion, xenemypos[1], yenemypos[m-4], 0);
						f++;
						e = 1;
					}
					else if ((pos > xenemypos[2]-40)&&(!(pos > xenemypos[2]+40))){
						al_draw_bitmap(explosion, xenemypos[2], yenemypos[m-3], 0);
						f++;
						e = 2;
					}
					else if ((pos > xenemypos[3]-40)&&(!(pos > xenemypos[3]+40))){
						al_draw_bitmap(explosion, xenemypos[3], yenemypos[m-2], 0);
						f++;
						e = 3;
					}
					else if ((pos > xenemypos[4]-40)&&(!(pos > xenemypos[4]+40))){
						al_draw_bitmap(explosion, xenemypos[4], yenemypos[m-1], 0);
						f++;
						e = 4;
					}
                         		if(!(f==0))
						al_play_sample(plosion, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

				}
	
				if((0 < f) && (f < 21)){
					f++;
					switch(e){
						case 0:
							al_draw_bitmap(explosion, xenemypos[0], yenemypos[m-5], 0);
							break;
						case 1:
							al_draw_bitmap(explosion, xenemypos[1], yenemypos[m-4], 0);
							break;
						case 2:
							al_draw_bitmap(explosion, xenemypos[2], yenemypos[m-3], 0);	
							break;
						case 3:
							al_draw_bitmap(explosion, xenemypos[3], yenemypos[m-2], 0);
							break;
						case 4:
							al_draw_bitmap(explosion, xenemypos[4], yenemypos[m-1], 0);
							break;
						}
				}
				if(f==20)
					f = 0;
			}
			if(step==0){
				al_draw_text(font, al_map_rgb(255, 255, 255), 200, 200, 0, "Welcome to the tutorial of Galactic Conflict");
				al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "press Enter to continue when you have read all on the current screen to continue");
				al_draw_text(font, al_map_rgb(255, 255, 255), 100, 400, 0, "you will use the left and right arrows to move the spaceship around\n\n");
			}
			if(step==1){
				al_draw_text(font, al_map_rgb(255, 255, 255), 200, 200, 0, "By the way, from now on you can press escape to exit");
				al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "Now, you can try moving around! \n\n (enter to continue)");
				al_draw_text(font, al_map_rgb(255, 255, 0), 100, 450, 0, "Oh and you will be using the space button to shoot!");
			}
			if(step==2){
                                al_draw_text(font, al_map_rgb(255, 255, 0), 200, 200, 0, "Try shooting now!");
				al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "next tutorial round we'll be adding enemies, which you have to shoot.");
				al_draw_text(font, al_map_rgb(255, 255, 255), 100, 450, 0, "(press enter to continue)");
			}
			if(step==3){
	                        al_draw_text(font, al_map_rgb(255, 255, 0), 200, 200, 0, "pow pow pow");
				al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "during the game you will be able to use the m key to open a music menu!");
				al_draw_text(font, al_map_rgb(255, 255, 255), 100, 450, 0, "(press enter to continue)");			
			}
			if(step==4){
				al_draw_text(font, al_map_rgb(255, 255, 255), 200, 200, 0, "select a first song (or nothing)");
			}
			if(shotnum==reqsh && step>1){
				reqsh++;
				n++;
				yshot[n] = 670;
				al_draw_bitmap(shot, xship, yshot[n], 0);
			}
			
			if(yshot[n]>0 && step > 1)
				al_draw_bitmap(shot, pos, yshot[n], 0); 

			al_flip_display();
			redraw = false;
		}
	}

	while(!done){
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
				if(!(yshot[n]<669))
					pos = xship;
		 		if(key[ALLEGRO_KEY_LEFT]){
					xship-=10;
				        if(xship<0)
						xship +=10;
				}
				if(key[ALLEGRO_KEY_RIGHT]){
					xship+=10;
				        if(xship>1435)
						xship -=10;							
				}

				if(key[ALLEGRO_KEY_ESCAPE])
					done = true;

				for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
					key[i] &= KEY_SEEN;
				yshot[n]-=10;
				x += dx;
				y += dy;

				if(x < 0){
					x *= -1;
					dx *= -1;
				}
				if(x > 1500){
					x -= (x - 1500) * 2;
					dx *= -1;
				}
				if(y < 0){
					y *= -1;
					dy *= -1;
				}
				if(y > 850){
					y -= (y - 850) * 2;
					dy *= -1;
				}

				dx *= 0.9;
				dy *= 0.9;
				redraw = true;
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
				dx += event.mouse.dx * 0.1;
				dy += event.mouse.dy * 0.1;
				al_set_mouse_xy(disp, 750, 425);
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if(event.keyboard.keycode == ALLEGRO_KEY_SPACE)
					shotnum++;
				key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				key[event.keyboard.keycode] &= KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;
			case ALLEGRO_EVENT_KEY_CHAR:
				break;
		}
	
		if(redraw && al_is_event_queue_empty(queue)){	
			al_clear_to_color(al_map_rgb(0, 0, 0));
	                al_draw_scaled_bitmap(spaceship, 0, 0, al_get_bitmap_width(spaceship), al_get_bitmap_height(spaceship),
			xship, yship, al_get_bitmap_width(spaceship) / 2, al_get_bitmap_width(spaceship) / 2, 0);    
			al_draw_textf(font, al_map_rgb(255,255,255), 0, 835, 0, "Score: %d", score); 

			for(int i = 0; i<5; i++){
				al_draw_bitmap(alienship1, xenemypos[i], yenemypos[m-5], 0);
				if(d == 0)
					xenemypos[i] += 10;
				if(xenemypos[4] == 1400 || xenemypos[3] == 1150 || xenemypos[2] == 900 || xenemypos[1] == 650 || xenemypos[0] == 400)
					d = 1;
				if(xenemypos[4] == 1100 || xenemypos[3] == 750 || xenemypos[2] == 500 || xenemypos[1] == 250 || xenemypos[0] == 0)
					d = 0;
				if(d == 1)
					xenemypos[i] -= 20;
			}

			if((yenemypos[m]==yshot[n])){
				if ((pos > xenemypos[0]-40)&&(!(pos > xenemypos[0]+40))){
					al_draw_bitmap(explosion, xenemypos[0], yenemypos[m-5], 0);
					f++;
					score++;
					e = 0;
				}
				else if ((pos > xenemypos[1]-40)&&(!(pos > xenemypos[1]+40))){
					al_draw_bitmap(explosion, xenemypos[1], yenemypos[m-4], 0);
					f++;
					score++;
					e = 1;
				}
				else if ((pos > xenemypos[2]-40)&&(!(pos > xenemypos[2]+40))){
                                   	al_draw_bitmap(explosion, xenemypos[2], yenemypos[m-3], 0);
					f++;
					score++;
					e = 2;
				}
				else if ((pos > xenemypos[3]-40)&&(!(pos > xenemypos[3]+40))){
					al_draw_bitmap(explosion, xenemypos[3], yenemypos[m-2], 0);
					f++;
					score++;
					e = 3;
				}
				else if ((pos > xenemypos[4]-40)&&(!(pos > xenemypos[4]+40))){
					al_draw_bitmap(explosion, xenemypos[4], yenemypos[m-1], 0);
					f++;
					score++;
					e = 4;
				}
				if(!(f==0))
					al_play_sample(plosion, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}

			if((0 < f) && (f < 21)){
				f++;
				switch(e){
					case 0:
						al_draw_bitmap(explosion, xenemypos[0], yenemypos[m-5], 0);
						break;
					case 1:
						al_draw_bitmap(explosion, xenemypos[1], yenemypos[m-4], 0);
						break;
					case 2:
						al_draw_bitmap(explosion, xenemypos[2], yenemypos[m-3], 0);	
						break;
					case 3:
						al_draw_bitmap(explosion, xenemypos[3], yenemypos[m-2], 0);
						break;
					case 4:
						al_draw_bitmap(explosion, xenemypos[4], yenemypos[m-1], 0);
						break;
				}
			}
			if(f==20)
				f = 0;
				
			if(shotnum==reqsh){
				reqsh++;
				n++;
				yshot[n] = 670;
				al_draw_bitmap(shot, xship, yshot[n], 0);
			}
			
			if(yshot[n]>0)
				al_draw_bitmap(shot, pos, yshot[n], 0); 

			al_flip_display();
			redraw = false;
		}
	}

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_bitmap(shot);
	al_destroy_bitmap(spaceship);
	al_destroy_bitmap(alienship1);
	al_destroy_bitmap(explosion);
	return 0;
}

