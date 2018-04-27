/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2018 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* A single pattern STEP requires two 32-bit registers. The have the form:
 * [red 10-bit value]   [green 10-bit value] [blue 10-bit value]  0x01
 * [white 10-bit value] [fade 10-bit value]  [dwell 10-bit value] 0x10
 * the last element in the pattern has the command bit (2 LSB bits) * of 11 to
 * signify EOF.
 *
 * Fade and dwell times have 2 components. The two MSB bits define
 * the time base:
 * 		00 = 100ms 		range of    0:25.5 -    0:00.1
 * 		01 = seconds	range of    4:15.0 -    0:01.0
 * 		10 = 5 seconds	range of   21:15.0 -    0:05.0
 * 		11 = minutes	range of 4:15:00.0 - 0:01:00.0
 *
 * The eight LSB bits of the time values define the actual value in the form:
 *
 * 		[8-bit value] * timebase
 *
 * For a pattern of greater resolution multiple pattern values should be used
 * with the same color values, a fade of 100 ms, and a the necessary dwell
 * time to achieve the desired total dwell.
 *
 * 		ie. for 5 minutes and 17.6 seconds
 * 			10 * 3C   //5 minutes
 * 			00 * B0   //17.6 seconds
 */

uint32_t pattern1 [] = {	(0x3FF << 22)|(0x000 << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x3FF << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x000 << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x000 << 12)|(0x000 << 2)| 0x1,
							(0x3FF << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x3FF << 22)|(0x3FF << 12)|(0x3FF << 2)| 0x1,
							(0x3FF << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x3
};

uint32_t pattern2 [] = {	(0x3FF << 22)|(0x3FF << 12)|(0x3FF << 2)| 0x1,
							(0x3FF << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x000 << 12)|(0x000 << 2)| 0x1,
							(0x3FF << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x000 << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x3FF << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x3FF << 22)|(0x000 << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x3
};

uint32_t pattern3 [] = {	(0x3FF << 22)|(0x3FF << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x3FF << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x3FF << 22)|(0x000 << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x3
};

uint32_t pattern4 [] = {	(0x3FF << 22)|(0x000 << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x3FF << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x000 << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x000 << 12)|(0x000 << 2)| 0x1,
							(0x3FF << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x3FF << 22)|(0x3FF << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x000 << 22)|(0x3FF << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x3FF << 22)|(0x000 << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x2,

							(0x3FF << 22)|(0x3FF << 12)|(0x3FF << 2)| 0x1,
							(0x3FF << 22)|(0x1 << 20)|(0x02 << 12)|(0x1 << 10)|(0x03 << 2)| 0x3
};

uint32_t pattern5 [] = {	(0x3FF << 22)|(0x000 << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x0 << 20)|(0x0A << 12)|(0x0 << 10)|(0x0A << 2)| 0x2,

							(0x000 << 22)|(0x3FF << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x0 << 20)|(0x0A << 12)|(0x0 << 10)|(0x0A << 2)| 0x2,

							(0x000 << 22)|(0x000 << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x0 << 20)|(0x0A << 12)|(0x0 << 10)|(0x0A << 2)| 0x2,

							(0x000 << 22)|(0x000 << 12)|(0x000 << 2)| 0x1,
							(0x3FF << 22)|(0x0 << 20)|(0x0A << 12)|(0x0 << 10)|(0x0A << 2)| 0x2,

							(0x3FF << 22)|(0x3FF << 12)|(0x000 << 2)| 0x1,
							(0x000 << 22)|(0x0 << 20)|(0x0A << 12)|(0x0 << 10)|(0x0A << 2)| 0x2,

							(0x000 << 22)|(0x3FF << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x0 << 20)|(0x0A << 12)|(0x0 << 10)|(0x0A << 2)| 0x2,

							(0x3FF << 22)|(0x000 << 12)|(0x3FF << 2)| 0x1,
							(0x000 << 22)|(0x0 << 20)|(0x0A << 12)|(0x0 << 10)|(0x0A << 2)| 0x2,

							(0x3FF << 22)|(0x3FF << 12)|(0x3FF << 2)| 0x1,
							(0x3FF << 22)|(0x0 << 20)|(0x0A << 12)|(0x0 << 10)|(0x0A << 2)| 0x3
};


int main(void) {
	HAL_Init();
	/*

	nextState = pattern1;*/

	/*	Name 		Base addresses 				Size (bytes)
		Page 123 	0x0801EC00 - 0x0801EFFF 	1 Kbyte
		Page 124 	0x0801F000 - 0x0801F3FF 	1 Kbyte
		Page 125 	0x0801F400 - 0x0801F7FF 	1 Kbyte
		Page 126 	0x0801F800 - 0x0801FBFF 	1 Kbyte
		Page 127 	0x0801FC00 - 0x0801FFFF 	1 Kbyte
	 */


	FLASH_EraseInitTypeDef FLASH_InitStruct;
	uint32_t sectorErr, baseAddress = 0x0801EC00;

	FLASH_InitStruct.TypeErase 		= FLASH_TYPEERASE_PAGES;
	FLASH_InitStruct.NbPages 		= 5;
	FLASH_InitStruct.PageAddress	= baseAddress;

	 HAL_FLASH_Unlock();

	 HAL_FLASHEx_Erase(&FLASH_InitStruct, &sectorErr);

	 uint8_t index;

	 for(index = 0; index<= sizeof(pattern1) || index >= 250; index++){
		 HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,baseAddress+(index*4),pattern1[index]);
	 }

	 baseAddress = 0x0801F000;

	 for(index = 0; index<= sizeof(pattern2) || index >= 250; index++){
		 HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,baseAddress+(index*4),pattern2[index]);
	 }

	 baseAddress = 0x0801F400;

	 for(index = 0; index<= sizeof(pattern3) || index >= 250; index++){
		 HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,baseAddress+(index*4),pattern3[index]);
	 }

	 baseAddress = 0x0801F800;

	 for(index = 0; index<= sizeof(pattern4) || index >= 250; index++){
		 HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,baseAddress+(index*4),pattern4[index]);
	 }

	 baseAddress = 0x0801FC00;

	 for(index = 0; index<= sizeof(pattern5) || index >= 250; index++){
		 HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,baseAddress+(index*4),pattern5[index]);
	 }

	 HAL_FLASH_Lock();

	while (1);

}
