#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MobilePhone{
    int idMobile;
    char model[50];
    double price;

    struct MobilePhone* next;
};

struct MobileShop{
    struct MobilePhone* mobileList;
    int size;
};

// Hàm lấy điện thoại di động tại chỉ số chỉ định
struct MobilePhone* getMobileAtIndex(struct MobilePhone* head, int index) {
    if (head == NULL) {
        return NULL;
    }

    int count = 0;
    struct MobilePhone* current = head;

    while (current != NULL) {
        if (count == index) {
            return current;  
        }
        count++;
        current = current->next;
    }

    return NULL; 
}

// Hàm đếm độ dài
int lengthList(struct MobilePhone*  head){
    int count = 0;
    while (head != NULL){
        count ++;
        head = head->next;
    } 
    return count;
}

// Hàm tìm kiếm sdt theo id bằng tìm kiếm nhị phân
struct MobilePhone* searchMobile(struct MobileShop* shop, int id) {
    int length = lengthList(shop->mobileList);
    struct MobilePhone* current = shop->mobileList;

    int low = 0;
    int high = length - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        struct MobilePhone* midMobile = getMobileAtIndex(current, mid);

        if (midMobile == NULL) {
            break; 
        }

        if (midMobile->idMobile == id) {
            return midMobile; 
        } else if (midMobile->idMobile < id) {
            low = mid + 1;  
        } else {
            high = mid - 1;  
        }
    }

    return NULL;
}


// Hàm thêm sdt
void addMobile(struct MobileShop* shop, int id, const char* model, double price) {
    // Kiểm tra xem id đã tồn tại hay không
    if (searchMobile(shop, id) != NULL) {
        printf("Error: Mobile with ID %d already exists\n", id);
        return;
    }

    // Tạo điện thoại di động mới
    struct MobilePhone* newMobile = (struct MobilePhone*)malloc(sizeof(struct MobilePhone));
    if (newMobile == NULL) {
        printf("Error add mobile\n");
        return;
    }

    // Đặt thông tin cho điện thoại di động mới
    newMobile->idMobile = id;
    strcpy(newMobile->model, model);
    newMobile->price = price;

    newMobile->next = NULL;

    // Kiểm tra xem mobileshop có trống hay không
    if (shop->mobileList == NULL) {
        shop->mobileList = newMobile;
    } else {
        struct MobilePhone* lastMobile = shop->mobileList;
        while (lastMobile->next != NULL) {
            lastMobile = lastMobile->next;
        }

        lastMobile->next = newMobile;
    }

    // Tăng kích thước của shop
    shop->size++;

    printf("Mobile add successful\n");
}


// Hàm cập nhật sdt
void updateMobile(struct MobileShop* shop, int id, const char* model, double price){
    struct MobilePhone* foundMobile = searchMobile(shop, id);

    if(foundMobile != NULL){
        strcpy( foundMobile ->model, model);
        foundMobile->price = price;

        printf("Mobile update sucessful\n");
    } else{
        printf("Mobile is not found \n");
    }
}

// Hàm xóa sdt
void deleteMobile(struct MobileShop* shop, int id){
    struct MobilePhone* current = shop ->mobileList;
    struct MobilePhone* prev = NULL;

    while (current != NULL && current->idMobile != id){
        prev = current;
        current = current->next;
    }

    if(current != NULL){
        if(prev == NULL){
            shop->mobileList = current->next;
        } else{
            prev->next = current->next;
        }

        free(current);
        // Giảm kích thước của shop
        shop->size--;

        printf("Mobile delete sucessful\n");
    } else{
        printf("Mobile no found\n");
    }
}

//Hàm đổi chỗ 
void swap(struct MobilePhone** a, struct MobilePhone** b){
    struct MobilePhone* temp = *a;
    *a = *b;
    *b = temp;
}

// Hàm phân vùng cho quicksort
int partition(struct MobilePhone** array, int low, int high){
    struct MobilePhone* pivot = array[high];
    int i = low -1;

    for(int j = low; j<high; j++){
        if(array[j]->idMobile < pivot->idMobile){
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i+1], &array[high]);
    return i+1;
}


// Hàm quicksort
void quicksort(struct MobilePhone** array, int low, int high){
    if(low < high){
        int pivotIndex = partition(array, low, high);
        quicksort(array, low, pivotIndex -1);
        quicksort(array, pivotIndex + 1, high);
    }
}


// Hàm in ra toàn bộ sdt
void printAllMobile(struct MobileShop* shop){
    if(shop->mobileList ==NULL){
        printf("Mobile list is empty");
        return ;
    }

    //Đếm số lượng sdt
    int count = 0;
    struct MobilePhone* current = shop->mobileList;
    while(current != NULL){
        count ++;
        current = current->next;
    }

    //Tạo mảng cho cửa hàng sdt
    struct MobilePhone** array = (struct MobilePhone**)malloc(count * sizeof(struct MobilePhone*));
    if(array == NULL){
        printf("Error memory allocation");
        return;
    }

    //Copy sdt sang mảng
    current = shop->mobileList;
    for(int i=0; i<count; i++){
        array[i] = current;
        current = current ->next;
    }

    // Sắp xếp theo quicksort
    quicksort(array, 0, count -1);

    printf("\n========== All Mobile Phone ==========\n");
    for(int i=0; i<count; i++){
        printf("Mobile ID: %d,\tModel: %s, \tPrice: %.2f\n", array[i]->idMobile, array[i]->model, array[i]->price);
    }

    free(array);
}


// Hàm heapify trong thuật toán heapSort cho linked list
void heapifyLinkedList(struct MobilePhone* head, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    struct MobilePhone* current = head;
    struct MobilePhone* largestNode = getMobileAtIndex(head, largest);

    if (left < n && getMobileAtIndex(head, left)->price > largestNode->price) {
        largest = left;
    }
    if (right < n && getMobileAtIndex(head, right)->price > largestNode->price) {
        largest = right;
    }
    if (largest != i) {
        swap(&getMobileAtIndex(head, i), &getMobileAtIndex(head, largest));
        heapifyLinkedList(head, n, largest);
    }
}

// Hàm sắp xếp heapSort cho linked list
void heapSortLinkedList(struct MobilePhone* head, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyLinkedList(head, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(&getMobileAtIndex(head, 0), &getMobileAtIndex(head, i));
        heapifyLinkedList(head, i, 0);
    }
}
// Hàm in top 5 sdt đắt nhất
void printTop5Mobile(struct MobileShop* shop) {
    if (shop->size < 5) {
        printf("Not enough mobiles to print top 5\n");
        return;
    }

    // Tạo mảng con trỏ để lưu trữ các điện thoại di động
    struct MobilePhone** tempArray = malloc(shop->size * sizeof(struct MobilePhone*));
    if (tempArray == NULL) {
        printf("Error memory allocation\n");
        return;
    }

    // Sao chép danh sách điện thoại di động vào mảng
    struct MobilePhone* current = shop->mobileList;
    for (int i = 0; i < shop->size; i++) {
        tempArray[i] = current;
        current = current->next;
    }

    // Sắp xếp mảng bằng heapSort cho linked list
    heapSortLinkedList(tempArray, shop->size);

    printf("Top 5 highest price mobile phones:\n");
    for (int i = 0; i < 5; i++) {
        printf("ID: %d, Model: %s, Price: %.2f\n", tempArray[i]->idMobile, tempArray[i]->model, tempArray[i]->price);
    }

    // Giải phóng bộ nhớ của mảng tạm thời
    free(tempArray);
}


//Hàm lưu file
void saveToFile(struct MobileShop* shop, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s for writing\n", filename);
        return;
    }

    struct MobilePhone* current = shop->mobileList;

    while (current != NULL) {
        fprintf(file, "%d %s %.2f\n", current->idMobile, current->model, current->price);
        current = current->next;
    }

    fclose(file);
    printf("Data saved to %s successful\n", filename);
}

// Hàm nạp dữ liệu khi mở lại chương trình
void loadToFile(struct MobileShop* shop, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s for reading\n", filename);
        return;
    }

    // Đọc dữ liệu từ file và thêm vào danh sách liên kết
    while (!feof(file)) {
        struct MobilePhone* newMobile = (struct MobilePhone*)malloc(sizeof(struct MobilePhone));
        if (newMobile == NULL) {
            printf("Error allocating memory.\n");
            fclose(file);
            return;
        }

        if (fscanf(file, "%d %s %lf", &newMobile->idMobile, newMobile->model, &newMobile->price) != 3) {
            free(newMobile);
            break;
        }

        newMobile->next = NULL;

        if (shop->mobileList == NULL) {
            shop->mobileList = newMobile;
        } else {
            struct MobilePhone* lastMobile = shop->mobileList;
            while (lastMobile->next != NULL) {
                lastMobile = lastMobile->next;
            }

            lastMobile->next = newMobile;
        }
    }

    fclose(file);
    printf("\nData loaded from %s successfully.\n", filename);
}

int main(){
    struct MobileShop shop;
    shop.mobileList = NULL;
    loadToFile(&shop, "mobile.txt");
    int choice;
    do{
        printf("\n----------- Mobile Phone Shop ------------ \n");
        printf("========================================== \n");
        printf("1. Add mobile phone \n");
        printf("2. Search mobile phone \n");
        printf("3. Update mobile phone\n");
        printf("4. Delete mobile phone \n");
        printf("5. Shop mobile \n");
        printf("0. Exit \n");
        printf("#Choise: ");
        scanf("%d", &choice);
        printf("========================================= \n");

        switch (choice)
        {
        case 1:{
            int id;
            char model[50];
            double price;
            printf("Enter mobile id: ");
            scanf("%d", &id);
            printf("Enter model: ");
            scanf("%s", model);
            printf("Enter price: ");
            scanf("%lf", &price);

            addMobile(&shop, id, model, price);
            break;
        }

        case 2:{
            int id;
            printf("Enter mobile id to search: ");
            scanf("%d", &id);
            struct MobilePhone* searchMobilePhone = searchMobile(&shop, id);
            if(searchMobilePhone != NULL){
                printf("Mobie to search is: \n");
                printf("ID: %d, Model: %s, Price: %.2f\n", searchMobilePhone->idMobile, searchMobilePhone->model, searchMobilePhone->price);
            } else{
                printf("Mobile is not found\n");
            }
            break;
        }

        case 3: {
            int id;
            char model[50];
            double price;

            printf("Enter mobile id to update: ");
            scanf("%d", &id);
            struct MobilePhone* foundMobile = searchMobile(&shop, id);
            if(foundMobile != NULL){
                printf ("Enter new model: ");
                scanf("%s", & model);
                printf("Enter new price: ");
                scanf("%ld", &price);

                updateMobile(&shop, id, model, price);
                printf("Mobile update successful\n");
            } else{
                printf("Mobile is not found\n");
            }
            break;
        }

        case 4:{
            int id;
            printf("Enter mobile to delete: ");
            scanf("%d", &id);

            deleteMobile(&shop, id);
            break;
        }

        case 5:{
            int reportChoice;
            do{
                printf("\n----------- Mobile Phone Shop --------- \n");
                    printf("======================================= \n");
                    printf("Shop Reports \n");
                    printf("1. Display all mobile phones \n");
                    printf("2. Display top 5 highest price mobile phones \n");
                    printf("0. Back to main menu \n");
                    printf("#Choose: ");
                    scanf("%d", &reportChoice);
                    printf("=======================================\n");

                    switch (reportChoice){
                        case 1:{
                            printAllMobile(&shop);
                            break;
                        }
                        case 2: {
                            printTop5Mobile(&shop);
                            break;
                        }
                        case 0:{
                            break;   
                        } 

                        default:
                            printf("Enter 0 to 2\n");
                    }
            }while (reportChoice != 0);
            break;
        }
        case 0:
            printf("Exit");
            break;
        default:
            printf("Enter 0 to 5 \n"); 
    } 
    }while (choice != 0);

    saveToFile(&shop, "mobile.txt");

    return 0;
}
