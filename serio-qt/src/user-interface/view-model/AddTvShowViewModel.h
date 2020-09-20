#ifndef SERIO_ADDTVSHOWVIEWMODEL_H
#define SERIO_ADDTVSHOWVIEWMODEL_H

#include "ViewModel.h"

namespace serio::qt {

class AddTvShowViewModel : public ViewModel {
public:
    explicit AddTvShowViewModel(StackOfViews& stack);
    void initialize(ActionRouter& router);
    void openView();
private:
    StackOfViews& stack;
};

}

#endif //SERIO_ADDTVSHOWVIEWMODEL_H
