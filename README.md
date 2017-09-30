# STM32_workshop
Repo de código e información acerca del taller de programación de microcontroladores ARM organizado por el IEA-UMSA

## Recursos

### Documentación técnica y de referencia

  - **[Datasheet.](http://www.st.com/resource/en/datasheet/stm32f103c8.pdf)** Este documento incluye información técnica, parámetros eléctricos y físicos del chip así como también información general de los periféricos.
  - **[Manual de referencia (stm32f103).](http://www.st.com/resource/en/reference_manual/cd00171190.pdf)** Este manual contiene información específica sobre todos los periféricos del chip, se necesita esta información para la utilización de los mismos.
  - **[Descripción de las librerías HAL.](http://www.st.com/resource/en/user_manual/dm00154093.pdf)** Este manual incluye información sobre la librería de abstracción de hardware (HAL) de stm32 para la familia stm32f1. Será el principal documento de referencia para la programación en C y C++.
  - **[Foro STM32.](https://community.st.com/community/stm32-community/stm32-forum)** En esta web se puede encontrar información brindada por la comunidad de desarrolladores de STM32 en todo el mundo. Se puede usar este recurso para hacer consultas y resolver dudas muy específicas acerca de los chips.

### Entorno de desarrollo

El entorno de desarrollo estará compuesto por un conjunto de paquetes de software para la configuración, edición, compilación, grabación y depuración de nuestros proyectos. Este entorno estará configurado sobre un sistema operativo Windows 7, 8, 8.1 o 10.

#### Herramientas de interfaz y configuración

> Se necesita crear una cuenta en la web de ST para descargar las siguientes herramientas.

ST cuenta con varias herramientas de software para realizar la interfaz con sus microcontroladores, las más importantes se listan a continuación:

  - **[Stlink utility.](http://www.st.com/en/development-tools/stsw-link004.html)** Este programa es una utilidad para cargar programas a los microcontroladores STM32 usando el depurador Stlink. Tambien cuenta con una herramienta de actualización de firmware para el mismo stlink.
  - **[Stm32 flash loader.](http://www.st.com/en/development-tools/flasher-stm32.html)** Esta herramienta permite grabar programas a la memoria flash de los microcontroladores usando el puerto serial y un adaptador usb-serial.
  - **[STMStudio.](http://www.st.com/en/development-tools/stm-studio-stm32.html)** Este programa permite monitorear variables y posiciones de memoria en tiempo real mediante tablas y gráficos a partir de archivos .elf usando el depurador ST-link.

Por otro lado, una de las tareas más tediosas en la programación de microcontroladores es la configuración inicial de periféricos, clock y otros aspectos específicos del hardware. Afortunadamente, ST cuenta con una utilidad que permite la configuración en un entorno gráfico del chip en su totalidad, mas la posibilidad de generar proyectos directamente a entornos de desarrollo populares. La herramienta se llama [STM32 CubeMX](http://www.st.com/en/development-tools/stm32cubemx.html) y esta disponible para sistemas operativos Windows y GNU/linux.

#### Entorno de programación

> Se necesita crear una cuenta para descargar el software

Para el entorno de programación usaremos el [IDE Keil uVision 5](https://www.keil.com/demo/eval/arm.htm) soportado oficialmente por ARM. Este IDE cuenta con un entornos de desarrollo integrado con las siguientes características:

  - Compilador de C y C++.
  - Editor de código fuente.
  - Herramientas de depuración y simulación.
  - Licencia de evaluación (hasta 32 KB de flash).
  - Soporte para una multitud de fabricantes y chips que usan la arquitectura ARM.

Por su parte, necesitaremos descargar paquetes específicos para la plataforma STM32 con los cuales podremos programar y depurar estos microcontroladores:

  - **[Keil uVision Software Packs.](https://www.keil.com/dd2/pack/#/eula-container)** Los paquetes de software de Keil contienen archivos de cabecera, drivers y utilidades específicas para cada familia y dispositivo, una vez en la página, se debe buscar el paquete correspondiente a la serie STM32F1 y descargarlo. Luego, instalar el paquete. Es posible también instalar los paquetes desde el entorno de Keil directamente de internet.
  - **[STM32F1 HAL Libraries.](http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-embedded-software/stm32cubef1.html)** Archivos de las librerías HAL. Contienen el código fuente, plantillas y ejemplos sobre lenguaje C. Este paquete se puede descargar también desde la herramienta Cube MX.

  


