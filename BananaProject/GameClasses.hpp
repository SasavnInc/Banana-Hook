#pragma once

struct __declspec(align(8)) UnityEngine_Object_Fields {
	intptr_t m_CachedPtr;
};

struct UnityEngine_MonoBehaviour_Fields : UnityEngine_Object_Fields {
	struct System_Threading_CancellationTokenSource_o* m_CancellationTokenSource;
};

struct UpdateMoneys_Fields : UnityEngine_MonoBehaviour_Fields {
	struct TMPro_TMP_Text_o* moneyText;
	struct TMPro_TMP_Text_o* clicksPerSecond;
	uint64_t money;
	bool tick;
	float tickFrame;
	int32_t timesClickedPerSecond;
	float framesPerSecond;
	struct UnityEngine_RectTransform_o* imageRectTransform;
	struct ClickAnimation_o* clickAnimation;
	struct UnityEngine_Canvas_o* to;
};

struct UpdateMoneys_o {
	class UpdateMoney_c* klass;
	void* monitor;
	UpdateMoneys_Fields fields;
};