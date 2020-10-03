import QtQuick 2.12
import '../animations'

RoundNullableImage {
    imageUrl: backgroundViewModel.image
    borderRadius: 0
    opacity: backgroundViewModel.visible ? 0.2 : 0
    Behavior on opacity {
        SlowNumberAnimation {}
    }
}
