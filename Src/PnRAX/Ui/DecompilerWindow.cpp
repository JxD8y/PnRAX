#include "DecompilerWindow.h"

DecompilerWindow::DecompilerWindow(GlobalContext& ctx,ImVec2 grid)
:ImGuiWindow("Decompiler - ",grid), m_gctx(ctx)
{
	this->SetWindowTitle(fmt::format("Decompiler - {}##{}", this->m_gctx.targetName, this->m_id));
}

void DecompilerWindow::Render() {
	ImGui::Begin(this->m_title.c_str(), &this->is_open);
	ImGui::Text("This is a place holder, here should be the decompiler");
	ImGui::End();
}