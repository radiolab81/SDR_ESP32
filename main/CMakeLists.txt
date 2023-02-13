set(COMPONENT_SRCS "SDR_ESP32.c" 
		   "./modules/globals.c"            

		   "./modules/gpiod.c"
		   "./modules/gpiod_rotary.c"
	
		   "./modules/display/ssd1306OLED/u8g2_esp32_hal.c"
		   "./modules/display/ssd1306OLED/displayd_i2c.c"

		   "./modules/iqreader.c"

		   "./modules/fft_audioelement.c"
		
		   "./modules/filter_audioelement.c"

		   "./modules/LPF6000_44k1.c"  
 	           "./modules/LPF4500_44k1.c"  
	           "./modules/LPF3200_44k1.c"   
                   "./modules/LPF500_44k1.c"	

		   "./modules/dc_block.c"	

 		   "./modules/demodulator_audioelement.c"

		   "./modules/agc_audioelement.c"
		    )

set(COMPONENT_ADD_INCLUDEDIRS "./modules")
set(EXTRA_COMPONENT_DIRS "./components")

register_component()


