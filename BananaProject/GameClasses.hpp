#pragma once

struct __declspec(align(8)) UnityEngine_Object_Fields {
	intptr_t m_CachedPtr;
};

struct UnityEngine_MonoBehaviour_Fields : UnityEngine_Object_Fields {
	struct System_Threading_CancellationTokenSource_o* m_CancellationTokenSource;
};

struct UpdateMoney_Fields : UnityEngine_MonoBehaviour_Fields {
	struct TMPro_TMP_Text_o* moneyText;
	struct TMPro_TMP_Text_o* clicksPerSecond;
	uint64_t moneyAmount;
	bool clickAccept; //idk, just some flag if deltaTimeClick >= 0.045
	float deltaTimeClick;
	int32_t CPS;
	float deltaTimeCSP;
	struct UnityEngine_RectTransform_o* imageRectTransform;
	struct ClickAnimation_o* clickAnimation;
	struct UnityEngine_Canvas_o* to;
};

struct UpdateMoney_o {
	class UpdateMoney_c* klass;
	void* monitor;
	UpdateMoney_Fields fields;
};