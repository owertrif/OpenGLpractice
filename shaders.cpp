#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Ініціалізація GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Створюємо вікно
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Context", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Прив'язка контексту

    // Ініціалізація GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    // Завершення роботи
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
