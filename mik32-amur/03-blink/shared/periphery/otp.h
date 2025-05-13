/**
 * @file
 * Контроллер блока однократно программируемой памяти OTP
 *
 * Данный блок предназначен для осуществления доступа к массиву блока antifuse OTP
 * на запись и чтение со стороны интерфейса APB. Данный блок является совместимым
 * с Hard IP antifuse OTP объемом 256 бит с организацией 8x32 otp_block_8x32_pwr_off,
 * разработанным для серийной технологии CMOSF8_4M_5V ПАО Микрон.
 */

#ifndef OTP_H_INCLUDED
#define OTP_H_INCLUDED

#define OTP_OTPDAT_OFFSET           0x00
#define OTP_OTPA_OFFSET             0x04
#define OTP_OTPCON_OFFSET           0x08
#define OTP_OTPSTA_OFFSET           0x0C
#define OTP_OTPDEC_OFFSET           0x10
#define OTP_OTPADJ_OFFSET           0x14
#define OTP_OTPWT1_OFFSET           0x18
#define OTP_OTPWT2_OFFSET           0x1C

//TODO: разобраться в полезности этих констант OTP register fields

#define OTP_APBNWS_S 0
#define OTP_BSY_S 0
#define OTP_N_RA_S 8
#define OTP_N_RSU_S 0
#define OTP_N_RH_S 16
#define OTP_SEL_UPP_READ 24
#define OTP_SEL_READ_CUR 27

//TODO: Перечень значений регистра Регистр адреса страницы (OTPA)

// Перечень значений регистра управления (OTPCON)

#define OTP_OTPCON_APBNWS_S 0
/// Отключить вставки тактов ожидания в процессе обмена по APB при чтении данных (обращении к регистру OTPDAT на чтение). Если такты ожидания отключены (APBNWS=1), то требуется производить опрос флага OTPSTA.BSY после записи адреса в OTPA до тех пор, пока EESTA.BSY не станет равным 0
#define OTP_OTPCON_APBNWS_M (0b1 << OTP_OTPCON_APBNWS_S)

#define OTP_OTPCON_MAN_WE_I_S 1
/// Включить режим ручного управления сигналом we i блока Hard IP. Служит для тестовых целей, использовать не рекомендуется
#define OTP_OTPCON_MAN_WE_I_M (0b1 << OTP_OTPCON_MAN_WE_I_S)

#define OTP_OTPCON_MAN_RE_I_S 2
/// Включить режим ручного управления сигналом re i блока Hard IP. Служит для тестовых целей, использовать не рекомендуется
#define OTP_OTPCON_MAN_RE_I_M (0b1 << OTP_OTPCON_MAN_RE_I_S)

#define OTP_OTPCON_MME_S 3
/// Включить режим ручного управления. В этом режиме автоматизированные операции чтения и записи не выполняются. Служит для тестовых целей, использовать не рекомендуется.
#define OTP_OTPCON_MME_M (0b1 << OTP_OTPCON_MME_S)

// Перечень значений регистра статуса (OTPSTA)

#define OTP_OTPSTA_BSY_S 0
/// Маска для проверки занятости блока (выполнения запрошенной операции)
#define OTP_OTPSTA_BSY_M (0b1 << OTP_OTPSTA_BSY_S)

// Перечень значений регистра бит дешифратора (OTPDEC)

#define OTP_OTPDEC_DECO_S 0
/// Маска для отображения выхода дешифратора строк блока antifuse OTP для целей тестирования
#define OTP_OTPDEC_DECO_M (0x3FF << OTP_OTPDEC_DECO_S)

// Перечень значений регистра управления временными параметрами (OTPADJ)

#define OTP_OTPADJ_N_RSU_S 0
/// Установить время между моментом начала транзакции на APB и положительным фронтом re i в тактах (рекомендуемый параметр).
#define OTP_OTPADJ_N_RSU_M (0b111 << OTP_OTPADJ_N_RSU_S)

#define OTP_OTPADJ_N_RA_S 8
/// Установить длительность высокого уровня сигнала re_i (вход HardIP) в тактах  (рекомендуемый параметр). 
#define OTP_OTPADJ_N_RA_M (0b111 << OTP_OTPADJ_N_RA_S)

#define OTP_OTPADJ_N_RH_S 16
/// Установить время между задним фронтом re_i и моментом, в который может быть начата новая транзакция на APB  (рекомендуемый параметр).
#define OTP_OTPADJ_N_RH_M (0b111 << OTP_OTPADJ_N_RH_S)

#define OTP_OTPADJ_SEL_UPP_READ_I_S 24
#define OTP_OTPADJ_SEL_UPP_READ_I_M (0b111 << OTP_OTPADJ_SEL_UPP_READ_I_S)
/// Установить напряжение на UPP матрице 2,0 В
#define OTP_OTPADJ_SEL_UPP_READ_I_2_0V_M (0b000 << OTP_OTPADJ_SEL_UPP_READ_I_S)
/// Установить напряжение на UPP матрице 2,5 В
#define OTP_OTPADJ_SEL_UPP_READ_I_2_5V_M (0b001 << OTP_OTPADJ_SEL_UPP_READ_I_S)
/// Установить напряжение на UPP матрице 3,0В
#define OTP_OTPADJ_SEL_UPP_READ_I_3_0V_M (0b011 << OTP_OTPADJ_SEL_UPP_READ_I_S)
/// Установить напряжение на UPP матрице VDD8
#define OTP_OTPADJ_SEL_UPP_READ_I_VDD18_M (0b010 << OTP_OTPADJ_SEL_UPP_READ_I_S)
/// Установить напряжение на UPP матрице VDD5
#define OTP_OTPADJ_SEL_UPP_READ_I_VDD5_M (0b110 << OTP_OTPADJ_SEL_UPP_READ_I_S)

#define OTP_OTPADJ_SEL_READ_CUR_I_S 27
/// Установить ток считывания 0,2 мкА (иначе – 2 мкА)
#define OTP_OTPADJ_SEL_READ_CUR_I_M (0b1 << OTP_OTPADJ_SEL_READ_CUR_I_S)

#define OTP_OTPADJ_POWER_OFF_I_S 28
/// Перевести Hard IP введен в режим пониженного энергопотребления, операции записи и чтения запрещены. 
#define OTP_OTPADJ_POWER_OFF_I_M (0b1 << OTP_OTPADJ_POWER_OFF_I_S)

// Перечень значений регистр подстройки длительности процедуры записи 1 (OTPWT1)

#define OTP_OTPWT1_N_SU_S 0
/// Установить время между моментом окончания транзакции на APB и положительным фронтом we i в тактах  (рекомендуемый параметр).
#define OTP_OTPWT1_N_SU_M (0b111 << OTP_OTPWT1_N_SU_S)

#define OTP_OTPWT1_N_H_S 8
/// Установить время между задним фронтом we_i и задним фронтом OTPSTA.BSY в тактах  (рекомендуемый параметр).
#define OTP_OTPWT1_N_H_M (0b111 << OTP_OTPWT1_N_H_S)

// Перечень значений для регистра подстройки длительности процедуры записи 2 (OTPWT2)

#define OTP_OTPWT2_N_W_S 0
/// Установить длительность высокого уровня сигнала we i (вход Hard IP) в тактах  (рекомендуемый параметр).
#define OTP_OTPWT2_N_W_M (0xFFFFFF << OTP_OTPWT2_N_W_S)

#ifndef __ASSEMBLER__
#include <inttypes.h>

/// @brief Структура регистров контроллера блока однократно программируемой памяти (OTP)
typedef struct
{
    volatile uint32_t OTPDAT;           ///< [RW]: Регистр чтения/записи данных. Биты [31:0] хранят данные из OTP. Запись в регистр запускает внутреннюю процедуру записи загруженных данных в массив antifuse OTP.
    volatile uint32_t OTPA;             ///< [RW]: Регистр адреса страницы. Биты [4:0] хранят адрес слова для выполнения чтения/записи.
    volatile uint32_t OTPCON;           ///< [RW]: Регистр управления. Биты [3:0] кодируют режимы управления.
    volatile uint32_t OTPSTA;           ///< [RO]: Регистр статуса занятости блока. Бит [0] кодирует статус занятости.
    volatile uint32_t OTPDEC;           ///< [RO]: Регистр бит дешифратора. Биты [8:0] отображают выход дешифратора строк блока antifuse OTP для целей тестирования.
    volatile uint32_t OTPADJ;           ///< [RW]: Регистр управления временными параметрами процедуры чтения и дополнительными настройками. Биты [2:0], [10:8], [18:16], [26:24], [27] и [28] используются для кодирования режимов работы.
    volatile uint32_t OTPWT1;           ///< [RW]: Регистр подстройки длительности процедуры записи 1. Биты [2:0] и [10:8] используются для кодирования режимов работы.
    volatile uint32_t OTPWT2;           ///< [RW]: Регистр подстройки длительности процедуры записи 2. Биты [23:0] хранят длительность высокого уровня сигнала we i (вход Hard IP) в тактах
} OTP_TypeDef;
#endif

#endif // OTP_H_INCLUDED
