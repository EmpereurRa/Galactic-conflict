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

int speed, x, y, br, nr, yshot, yshot2, yshot3, pos, pos2, pos3;
float dx, dy;
bool shot1, shot2, shot3 = 0;
int score, e, f, d, n, shotnum, shotnum2, shotnum3, temp, step = 0;
int reqsh, reqsh2, reqsh3 = 1;
int xship, yship = 750;
int xenemypos[5] = {150, 400, 650, 900, 1150};
int m = 10;

void enter_speed(){
	printf("enter the desired game speed (can be: 0-100)(recommended: 40-60): ");
	scanf("%d", &speed);
	printf("\n");
}

int main(){
	enter_speed();
	if(speed>100||speed<0){
		printf("not a valid speed.\n");
		enter_speed();
	}

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

	ALLEGRO_SAMPLE* song1 = al_load_sample("msong.ogg");
	pls_init(song1, "mysterious song");

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
			case ALLEGRO_EVENT_MOUSE_AXES:
				dx += event.mouse.dx * 0.1;
				dy += event.mouse.dy * 0.1;
				al_set_mouse_xy(disp, 750, 425);
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
			if (step==4){
				if(y>800)
					al_play_sample(song1, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			if (step>2&&step<4){
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
		if(temp == 0){
			al_play_sample(song1, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		        temp++;
		}
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_TIMER:
				if(!(yshot<669))
					pos = xship;
				if(!(yshot2<669))
					pos2 = xship;
				if(!(yshot3<669))
					pos3 = xship;
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
				if(shot2==1)
					yshot2-=10;
				if(shot3==1)
					yshot3-=10;
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
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){
					if(shot1==0&&shot2==0&&shot3==0){
						shot1=1;
					        shotnum++;
					}
					if(shot1==1&&shot2==0&&shot3==0){
						shot2=1;
						shotnum2++;
					}
					if(shot1==1&&shot2==1&&shot3==0){
						shot3=1;
					        shotnum3++;
					}
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
				if (((pos > xenemypos[0]-65)&&(!(pos > xenemypos[0]+15)))||((pos2 > xenemypos[0]-65)&&(!(pos2 > xenemypos[0]+15)))||((pos3 > xenemypos[0]-65)&&(!(pos3 > xenemypos[0]+15)))){
					al_draw_bitmap(explosion, xenemypos[0], 0, 0);
					f++;
					score++;
					e = 0;
				}
				else if (((pos > xenemypos[1]-65)&&(!(pos > xenemypos[1]+15)))||((pos2 > xenemypos[1]-65)&&(!(pos2 > xenemypos[1]+15)))||((pos3 > xenemypos[1]-65)&&(!(pos3 > xenemypos[1]+15)))){
					if((pos > xenemypos[1]-65)&&(!(pos > xenemypos[1]+15)))
						shot1=0;
					if((pos2 > xenemypos[1]-65)&&(!(pos2 > xenemypos[1]+15)))
						shot2=0;
					if((pos3 > xenemypos[1]-65)&&(!(pos3 > xenemypos[1]+15)))
						shot3=0;
					al_draw_bitmap(explosion, xenemypos[1], 0, 0);
					f++;
					score++;
					e = 1;
				}
				else if (((pos > xenemypos[2]-65)&&(!(pos > xenemypos[2]+15)))||((pos2 > xenemypos[2]-65)&&(!(pos2 > xenemypos[2]+15)))||((pos3 > xenemypos[2]-65)&&(!(pos3 > xenemypos[2]+15)))){
                                   	al_draw_bitmap(explosion, xenemypos[2], 0, 0);
					f++;
					score++;
					e = 2;
				}
				else if (((pos > xenemypos[3]-65)&&(!(pos > xenemypos[3]+15)))||((pos2 > xenemypos[3]-65)&&(!(pos2 > xenemypos[3]+15)))||((pos3 > xenemypos[3]-65)&&(!(pos3 > xenemypos[3]+15)))){
					al_draw_bitmap(explosion, xenemypos[3], 0, 0);
					f++;
					score++;
					e = 3;
				}
				else if (((pos > xenemypos[4]-65)&&(!(pos > xenemypos[4]+15)))||((pos2 > xenemypos[4]-65)&&(!(pos2 > xenemypos[4]+15)))||((pos3 > xenemypos[4]-65)&&(!(pos3 > xenemypos[4]+15)))){
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
				printf("shot 1");
				reqsh++;
				n++;
				if(shot1==1){
					yshot = 670;
					al_draw_bitmap(shot, xship+26, yshot, 0);	
				}
			}
			if(shotnum2==reqsh2){
				printf("shot 2");
				reqsh2++;
	                        nr++;
				if(shot2==1){
					yshot2 = 670;
					al_draw_bitmap(shot, xship+26, yshot2, 0);
				}
			}
			if(shotnum3==reqsh3){
				printf("shot 3");
				reqsh3++;
				br++;
				if(shot3==1){
					yshot3 = 670;
					al_draw_bitmap(shot, xship+26, yshot3, 0);			
				}
			}

			if(shot1==1 && shotnum+1 == reqsh)
				al_draw_bitmap(shot, pos+26, yshot, 0);
			if(shot2==1 && shotnum2+1 == reqsh2)
				al_draw_bitmap(shot, pos+26, yshot2, 0);
			if(shot3==1 && shotnum3+1 == reqsh3)
				al_draw_bitmap(shot, pos+26, yshot3, 0);

			if(yshot<0)
				shot1=0;
			if(yshot2<0)
				shot2=0;
			if(yshot3<0)
				shot3=0;
			al_flip_display();
			redraw = false;
		}
	}

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_sample(plosion);
	al_destroy_sample(song1);
	al_uninstall_audio();
	al_destroy_event_queue(queue);
	al_destroy_bitmap(shot);
	al_destroy_bitmap(spaceship);
	al_destroy_bitmap(alienship1);
	al_destroy_bitmap(explosion);
	return 0;
}

