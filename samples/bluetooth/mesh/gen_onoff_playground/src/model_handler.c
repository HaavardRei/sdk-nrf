#include <zephyr/bluetooth/bluetooth.h>
#include <bluetooth/mesh/models.h>
#include <dk_buttons_and_leds.h>
#include "model_handler.h"

/** Context for a single switch. */
struct button {
	/** Current status of the corresponding server. */
	bool status;
	/** Generic OnOff client instance for this switch. */
	struct bt_mesh_onoff_cli client;
};
/*Use LEDs to reflect the incoming message's OnOff value, here: 
Set handler has led index 0 and status handler has led index 1*/

static void status_handler(struct bt_mesh_onoff_cli *cli, struct bt_mesh_msg_ctx *ctx,
			   const struct bt_mesh_onoff_status *status);

static void gen_set(struct bt_mesh_onoff_srv *srv, struct bt_mesh_msg_ctx *ctx,
		    const struct bt_mesh_onoff_set *set, struct bt_mesh_onoff_status *rsp);

static void gen_get(struct bt_mesh_onoff_srv *srv, struct bt_mesh_msg_ctx *ctx,
		    struct bt_mesh_onoff_status *rsp);

static const struct bt_mesh_onoff_srv_handlers onoff_handlers = {
	.set = gen_set,
	.get = gen_get,
};

static struct bt_mesh_onoff_srv onoff_srv = BT_MESH_ONOFF_SRV_INIT(&onoff_handlers);
static struct button button = {
	.client = BT_MESH_ONOFF_CLI_INIT(&status_handler),
};

static void gen_set(struct bt_mesh_onoff_srv *srv, struct bt_mesh_msg_ctx *ctx,
		    const struct bt_mesh_onoff_set *set, struct bt_mesh_onoff_status *rsp)
{
	rsp->present_on_off = set->on_off;
	dk_set_led(0, set->on_off);
	printk("Set handler: Recieved set message: %s\n", set->on_off ? "On" : "Off");
}

static void gen_get(struct bt_mesh_onoff_srv *srv, struct bt_mesh_msg_ctx *ctx,
		    struct bt_mesh_onoff_status *rsp)
{
	// Not implemented
}

static void status_handler(struct bt_mesh_onoff_cli *cli, struct bt_mesh_msg_ctx *ctx,
			   const struct bt_mesh_onoff_status *status)
{
	dk_set_led(1, status->present_on_off);
	printk("Status handler: Recieved status message: %s\n",
	       status->present_on_off ? "On" : "Off");
}

static void button_handler_cb(uint32_t pressed, uint32_t changed)
{
	if (!bt_mesh_is_provisioned()) {
		return;
	}
	if (pressed & changed & BIT(0)) {
		struct bt_mesh_onoff_set set = {
			.on_off = !button.status,
		};
		int err = bt_mesh_onoff_cli_set(&button.client, NULL, &set, NULL);
		if (!err) {
			button.status = set.on_off; 
		} else {
			printk("OnOff set failed.\n");
		}
	}
}

static void attention_on(struct bt_mesh_model *mod)
{
	printk("Attention On\n");
}

static void attention_off(struct bt_mesh_model *mod)
{
	printk("Attention Off\n");
}

static const struct bt_mesh_health_srv_cb health_srv_cb = {
	.attn_on = attention_on,
	.attn_off = attention_off,
};

static struct bt_mesh_health_srv health_srv = {
	.cb = &health_srv_cb,
};

BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);

static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(1,
		     BT_MESH_MODEL_LIST(
			     BT_MESH_MODEL_CFG_SRV,
			     BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
			     BT_MESH_MODEL_ONOFF_CLI(&button.client)),
		     BT_MESH_MODEL_NONE),
	BT_MESH_ELEM(2,
		     BT_MESH_MODEL_LIST(
			     BT_MESH_MODEL_ONOFF_SRV(&onoff_srv)),
		     BT_MESH_MODEL_NONE),

};

static const struct bt_mesh_comp comp = {
	.cid = CONFIG_BT_COMPANY_ID,
	.elem = elements,
	.elem_count = ARRAY_SIZE(elements),
};

const struct bt_mesh_comp *model_handler_init(void)
{
	static struct button_handler button_handler = {
		.cb = button_handler_cb,
	};

	dk_button_handler_add(&button_handler);

	return &comp;
}
