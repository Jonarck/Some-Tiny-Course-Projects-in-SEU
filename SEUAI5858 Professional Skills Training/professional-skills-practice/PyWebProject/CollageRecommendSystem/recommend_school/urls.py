from django.urls import path, re_path
from . import views


urlpatterns = [
    path('recommend_school/', views.recommend_school, name='recommend_school'),
]