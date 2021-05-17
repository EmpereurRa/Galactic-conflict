#include<stdio.h>
#include<stdlib.h>
#include<allegro5/allegro5.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>

/*to do:
dunno, may one day make a better thing with an 
actual challange and story or try to do this in
another programming language or I may even try
to make this a browser application.
*/

void pls_init(bool test, const char *description){
	if(test) return;

	printf("couldn't initialize %s\n", description);
	exit(1);
}

int width = 850;
int length = 1500;
int speed, x, y, yshot, pos;
float dx, dy;
bool shot1 = 0;
int score, e, f, d, shotnum, music, step = 0;
int reqsh = 1;
int xship, yship = 750;
int xenemypos[5] = {150, 400, 650, 900, 1150};
int m = 10;

void enter_speed(){
	printf("enter the desired game speed (can be: 0-200)(recommended: 40-60): ");
	scanf("%d", &speed);
}

int main(){
	enter_speed();
	while(speed>200||speed<0){
		printf("not a valid speed.\n\n");
		enter_speed();
	}
	printf("\nloading...\n");
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

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / speed);
	pls_init(timer, "timer");

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	pls_init(queue, "queue");

	ALLEGRO_SAMPLE* plosion = al_load_sample("enemy-down.ogg");
	pls_init(plosion, "explosion sound");

	printf("20/100\n");
	ALLEGRO_SAMPLE* song1 = al_load_sample("msong.ogg");
	pls_init(song1, "mysterious song");

	ALLEGRO_SAMPLE* song2 = al_load_sample("fun song.ogg");
	pls_init(song2, "fun song");
	printf("50/100\n");

	ALLEGRO_SAMPLE* song3 = al_load_sample("trouble song.ogg");
	pls_init(song3, "trouble song");
	printf("65/100\n");

	ALLEGRO_SAMPLE* song4 = al_load_sample("silent song 1.ogg");
	pls_init(song4, "silent song");
	printf("80/100\n");
	
	ALLEGRO_SAMPLE* song4_5 = al_load_sample("silent song 2.ogg");
	pls_init(song4_5, "silent song 2");

	printf("done!\n");

	ALLEGRO_DISPLAY* disp = al_create_display(length, width);
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
	ALLEGRO_EVENT event;

	#define KEY_SEEN     1
	#define KEY_RELEASED 2

	unsigned char key[ALLEGRO_KEY_MAX];
	memset(key, 0, sizeof(key));

	al_grab_mouse(disp);
	al_start_timer(timer);
	while(!introdone){
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
				if(!(yshot<669) && step > 1)
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
					yshot-=10;

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
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				if(step==4){
					printf("x: %d, y: %d\n", x, y);
					printf("dx: %f, dy: %f\n", dx, dy);
					if(y<101){
						al_stop_samples();
						al_play_sample(song1, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y<201&&y>100){
						al_stop_samples();
						al_play_sample(song2, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y<301&&y>200){
						al_stop_samples();
						al_play_sample(song3, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y<401&&y>300){
						al_stop_samples();
						al_play_sample(song4, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y<501&&y>400){
						al_stop_samples();
						al_play_sample(song4_5, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y>699)
						al_stop_samples();
				}
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
			        al_set_mouse_xy(disp, x, y);
				dx += event.mouse.dx * 0.1;
				dy += event.mouse.dy * 0.1;
				break;			
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(disp);
				al_flip_display();
			        break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if((event.keyboard.keycode == ALLEGRO_KEY_SPACE) && step>1 && !(step>3))
					shotnum++;
				if((event.keyboard.keycode == ALLEGRO_KEY_ENTER)){
					step++;
					if(step == 5)
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
			if(y<101&&step==4){
				al_draw_filled_rectangle(1500, 0, 0, 100, al_map_rgba_f(0, 0, 0.5, 0.5));
				al_draw_text(font, al_map_rgb(255, 255, 25), 1500/2, 50, ALLEGRO_ALIGN_CENTER, "mysterious song");
			}
			if(y<201&&y>100&&step==4){
				al_draw_filled_rectangle(1500, 100, 0, 200, al_map_rgba_f(0, 0.5, 0, 0.5));
				al_draw_text(font, al_map_rgb(255, 25, 255), 1500/2, 150, ALLEGRO_ALIGN_CENTER, "fun song");
			}
			if(y<301&&y>200&&step==4){
				al_draw_filled_rectangle(1500, 200, 0, 300, al_map_rgba_f(0, 0.5, 0.5, 0.5));
				al_draw_text(font, al_map_rgb(25, 255, 25), 1500/2, 250, ALLEGRO_ALIGN_CENTER, "trouble song");
			}
			if(y<401&&y>300&&step==4){
				al_draw_filled_rectangle(1500, 300, 0, 400, al_map_rgba_f(0.5, 0, 0, 0.5));
				al_draw_text(font, al_map_rgb(255, 255, 255), 1500/2, 350, ALLEGRO_ALIGN_CENTER, "silent song part 1");
			}
			if(y<501&&y>400&&step==4){
				al_draw_filled_rectangle(1500, 400, 0, 500, al_map_rgba_f(0.5, 0.5, 0, 0.5));
				al_draw_text(font, al_map_rgb(255, 25, 25), 1500/2, 450, ALLEGRO_ALIGN_CENTER, "silent song part 2");
			}
			if(y>699&&step==4){
				al_draw_filled_rectangle(1500, 700, 0, 850, al_map_rgba_f(0.5, 0.5, 0.5, 0.5));
				al_draw_text(font, al_map_rgb(0, 0, 0), 1500/2, 775, ALLEGRO_ALIGN_CENTER, "stop all songs!");
			}
			if(step>2&&step<4){
				for(int i = 0; i<5; i++){
					al_draw_bitmap(alienship1, xenemypos[i], 0, 0);
					if(d == 0)
						xenemypos[i] += 10;
					if(xenemypos[4] == 1400 || xenemypos[3] == 1150 || xenemypos[2] == 900 || xenemypos[1] == 650 || xenemypos[0] == 400)
						d = 1;
					if(xenemypos[4] == 1100 || xenemypos[3] == 750 || xenemypos[2] == 500 || xenemypos[1] == 250 || xenemypos[0] == 0)
						d = 0;
					if(d == 1)
						xenemypos[i] -= 20;
				}
				if((0==yshot)){
					if ((pos > xenemypos[0]-65)&&(!(pos > xenemypos[0]+15))){
						al_draw_bitmap(explosion, xenemypos[0], 0, 0);
						f++;
						e = 0;
					}
					else if ((pos > xenemypos[1]-65)&&(!(pos > xenemypos[1]+15))){
						al_draw_bitmap(explosion, xenemypos[1], 0, 0);
						f++;
						e = 1;
					}
					else if ((pos > xenemypos[2]-65)&&(!(pos > xenemypos[2]+15))){
						al_draw_bitmap(explosion, xenemypos[2], 0, 0);
						f++;
						e = 2;
					}
					else if ((pos > xenemypos[3]-65)&&(!(pos > xenemypos[3]+15))){
						al_draw_bitmap(explosion, xenemypos[3], 0, 0);
						f++;
						e = 3;
					}
					else if ((pos > xenemypos[4]-65)&&(!(pos > xenemypos[4]+15))){
						al_draw_bitmap(explosion, xenemypos[4], 0, 0);
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
							al_draw_bitmap(explosion, xenemypos[0], 0, 0);
							break;
						case 1:
							al_draw_bitmap(explosion, xenemypos[1], 0, 0);
							break;
						case 2:
							al_draw_bitmap(explosion, xenemypos[2], 0, 0);	
							break;
						case 3:
							al_draw_bitmap(explosion, xenemypos[3], 0, 0);
							break;
						case 4:
							al_draw_bitmap(explosion, xenemypos[4], 0, 0);
							break;
						}
				}
				if(f==20)
					f = 0;
			}
			
			switch(step){
				case 0:
					al_draw_text(font, al_map_rgb(255, 255, 255), 200, 200, 0, "Welcome to the tutorial of Galactic Conflict");
					al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "press Enter to continue when you have read all on the current screen to continue");
					al_draw_text(font, al_map_rgb(255, 255, 255), 100, 400, 0, "you will use the left and right arrows to move the spaceship around\n\n");
					break;
			        case 1:
					al_draw_text(font, al_map_rgb(255, 255, 255), 200, 200, 0, "By the way, from now on you can press escape to exit");
					al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "Now, you can try moving around! \n\n (enter to continue)");
					al_draw_text(font, al_map_rgb(255, 255, 0), 100, 450, 0, "Oh and you will be using the space button to shoot!");
					break;
				case 2:
					al_draw_text(font, al_map_rgb(255, 255, 0), 200, 200, 0, "Try shooting now!");
					al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "next tutorial round we'll be adding enemies, which you have to shoot.");
					al_draw_text(font, al_map_rgb(255, 255, 255), 100, 450, 0, "(press enter to continue)");
					break;
				case 3:
					al_draw_text(font, al_map_rgb(255, 255, 0), 200, 200, 0, "pow pow pow");
					al_draw_text(font, al_map_rgb(255, 255, 0), 200, 250, 0, "during the game you will be able to use the m key to open a music menu!");
					al_draw_text(font, al_map_rgb(255, 255, 255), 100, 450, 0, "(for which to exit while in it you'll have to use m, what? it's self-explainatory?)(press enter to continue)");			
					break;
			        case 4:
					al_draw_text(font, al_map_rgb(255, 255, 255), 1500/2, 0, ALLEGRO_ALIGN_CENTER, "select a first song (or nothing)");
					break;
			}
			if(shotnum==reqsh && step>1){
				reqsh++;
				yshot = 670;
				al_draw_bitmap(shot, xship+26, yshot, 0);
			}
			if(yshot>0 && step > 1)
				al_draw_bitmap(shot, pos+26, yshot, 0); 

			al_flip_display();
			redraw = false;
		}
	}

	while(!done){	
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
				if(!(yshot<669))
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

				if(shot1==1)
					yshot-=10;

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
				al_set_mouse_xy(disp, x, y);
				dx += event.mouse.dx * 0.1;
				dy += event.mouse.dy * 0.1;
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				if(music==1){
					printf("x: %d, y: %d\n", x, y);
					printf("dx: %f, dy: %f\n", dx, dy);
					if(y<101){
						al_stop_samples();
						al_play_sample(song1, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y<201&&y>100){
						al_stop_samples();
						al_play_sample(song2, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y<301&&y>200){
						al_stop_samples();
						al_play_sample(song3, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y<401&&y>300){
						al_stop_samples();
						al_play_sample(song4, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y<501&&y>400){
						al_stop_samples();
						al_play_sample(song4_5, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
					}
					if(y>699)
						al_stop_samples();
				}
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){
					if(shot1==0){
						shot1=1;
					        shotnum++;
					}
				}
				if(event.keyboard.keycode == ALLEGRO_KEY_M){
					if(music==0)
						music=1;
					else if(music==1)
						music=0;
				}
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
			if(music==0){
				al_draw_textf(font, al_map_rgb(255,255,255), 0, 835, 0, "Score: %d", score); 	
				for(int i = 0; i<5; i++){
					al_draw_bitmap(alienship1, xenemypos[i], 0, 0);
					if(d == 0)
						xenemypos[i] += 10;
					if(xenemypos[4] == 1400 || xenemypos[3] == 1150 || xenemypos[2] == 900 || xenemypos[1] == 650 || xenemypos[0] == 400)
						d = 1;
					if(xenemypos[4] == 1100 || xenemypos[3] == 750 || xenemypos[2] == 500 || xenemypos[1] == 250 || xenemypos[0] == 0)
						d = 0;
					if(d == 1)
						xenemypos[i] -= 20;
				}
				if((0==yshot)){
					if ((pos > xenemypos[0]-65)&&(!(pos > xenemypos[0]+15))){
						al_draw_bitmap(explosion, xenemypos[0], 0, 0);
						f++;
						score++;
						e = 0;
					}
					else if ((pos > xenemypos[1]-65)&&(!(pos > xenemypos[1]+15))){
						al_draw_bitmap(explosion, xenemypos[1], 0, 0);
						f++;
						score++;
						e = 1;
					}
					else if ((pos > xenemypos[2]-65)&&(!(pos > xenemypos[2]+15))){
						al_draw_bitmap(explosion, xenemypos[2], 0, 0);
						f++;
						score++;
						e = 2;
					}
					else if ((pos > xenemypos[3]-65)&&(!(pos > xenemypos[3]+15))){
						al_draw_bitmap(explosion, xenemypos[3], 0, 0);
						f++;
						score++;
						e = 3;
					}
					else if ((pos > xenemypos[4]-65)&&(!(pos > xenemypos[4]+15))){
						al_draw_bitmap(explosion, xenemypos[4], 0, 0);
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
							al_draw_bitmap(explosion, xenemypos[0], 0, 0);
								break;
						case 1:
							al_draw_bitmap(explosion, xenemypos[1], 0, 0);
							break;
						case 2:
							al_draw_bitmap(explosion, xenemypos[2], 0, 0);	
							break;
						case 3:
							al_draw_bitmap(explosion, xenemypos[3], 0, 0);
							break;
						case 4:
							al_draw_bitmap(explosion, xenemypos[4], 0, 0);
							break;
					}
				}
				if(f==20)
					f = 0;
				if(shotnum==reqsh){
					//printf("shot 1");
					reqsh++;
					if(shot1==1){
						yshot = 670;
						al_draw_bitmap(shot, xship+26, yshot, 0);	
					}
				}
				if(shot1==1 && shotnum+1 == reqsh)
					al_draw_bitmap(shot, pos+26, yshot, 0);
	
				if(yshot<0)
					shot1=0;
			}
			if(music==1){
				if(y<101){
					al_draw_filled_rectangle(1500, 0, 0, 100, al_map_rgba_f(0, 0, 0.5, 0.5));
					al_draw_text(font, al_map_rgb(255, 255, 25), 1500/2, 50, ALLEGRO_ALIGN_CENTER, "mysterious song");
				}
				if(y<201&&y>100){
					al_draw_filled_rectangle(1500, 100, 0, 200, al_map_rgba_f(0, 0.5, 0, 0.5));
					al_draw_text(font, al_map_rgb(255, 25, 255), 1500/2, 150, ALLEGRO_ALIGN_CENTER, "fun song");
				}
				if(y<301&&y>200){
					al_draw_filled_rectangle(1500, 200, 0, 300, al_map_rgba_f(0, 0.5, 0.5, 0.5));
					al_draw_text(font, al_map_rgb(25, 255, 25), 1500/2, 250, ALLEGRO_ALIGN_CENTER, "trouble song");
				}
				if(y<401&&y>300){
					al_draw_filled_rectangle(1500, 300, 0, 400, al_map_rgba_f(0.5, 0, 0, 0.5));
					al_draw_text(font, al_map_rgb(255, 255, 255), 1500/2, 350, ALLEGRO_ALIGN_CENTER, "silent song part 1");
				}
				if(y<501&&y>400){
					al_draw_filled_rectangle(1500, 400, 0, 500, al_map_rgba_f(0.5, 0.5, 0, 0.5));
					al_draw_text(font, al_map_rgb(255, 25, 25), 1500/2, 450, ALLEGRO_ALIGN_CENTER, "silent song part 2");
				}
				if(y>699){
					al_draw_filled_rectangle(1500, 700, 0, 850, al_map_rgba_f(0.5, 0.5, 0.5, 0.5));
					al_draw_text(font, al_map_rgb(0, 0, 0), 1500/2, 775, ALLEGRO_ALIGN_CENTER, "stop all songs!");
           			}
			}
			al_flip_display();
			redraw = false;
		}
	}

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_sample(plosion);
	al_destroy_sample(song1);
	al_destroy_sample(song2);
	al_destroy_sample(song3);
	al_destroy_sample(song4);
	al_destroy_sample(song4_5);
	al_uninstall_audio();
	al_destroy_event_queue(queue);
	al_destroy_bitmap(shot);
	al_destroy_bitmap(spaceship);
	al_destroy_bitmap(alienship1);
	al_destroy_bitmap(explosion);
	return 0;
}

