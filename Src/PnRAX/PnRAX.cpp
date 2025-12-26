#include <iostream>
#include "Ui/ImGui_Impl_easy.h"

void blankWindow() {

}
int main(int argc, char* argv[]) {
    Imeasy::AddRenderingObject(&blankWindow);
    Imeasy::StartRendering(
    "Test Window",800,600);
    return 0;
}