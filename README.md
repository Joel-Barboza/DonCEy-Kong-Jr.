# DonCEy-Kong-Jr

Juego multijugador inspirado en **Donkey Kong Jr.**, desarrollado en **C
(cliente)** y **Java (servidor)**.\
El jugador controla a Donkey Kong Jr. para rescatar a su padre,
esquivando cocodrilos y recolectando frutas.

> ⚠️ **Nota:** SDL3 se usa como un submódulo.

------------------------------------------------------------------------

## 🛠️ Clonar repositorio

``` bash
git clone --recurse-submodules https://github.com/Joel-Barboza/DonCEy-Kong-Jr.git
```

------------------------------------------------------------------------

## 🔄 Actualizar repositorio

``` bash
git pull --recurse-submodules
git submodule update --init --recursive
```

------------------------------------------------------------------------

## ⚙️ Configuración del Servidor Java en IntelliJ

### 1. Agregar el JAR de JSON a IntelliJ

1.  Abre **IntelliJ IDEA**
2.  Menú superior: **File → Project Structure** (o
    `Ctrl + Alt + Shift + S`)
3.  En el panel izquierdo selecciona **Modules**
4.  Selecciona tu módulo (probablemente "Servidor")
5.  Ve a la pestaña **Dependencies**
6.  Haz clic en **+ → JARs or Directories...**
7.  Selecciona el archivo **json-20240303.jar**
8.  Presiona **Apply → OK**

------------------------------------------------------------------------

### 2. Marcar carpeta *resources* como Resource Root

**Opción A:**

1.  En el panel *Project* de IntelliJ
2.  Navega a `Servidor/src/main/resources`
3.  Clic derecho → **Mark Directory as → Resources Root**

**Opción B (alternativa):**

1.  Ve a **File → Project Structure**
2.  En el panel izquierdo selecciona **Modules**
3.  Expande tu módulo y navega a `Servidor/src/main/resources`
4.  Clic derecho → **Resources**
5.  Presiona **Apply → OK**
